(function (global) {
'use strict';

// A special symbol to hide private members.
const _ = Symbol();

// MAGIC2 Commands.
const C_LINE = 0x00;
const C_SPLINE = 0x01;
const C_BOX = 0x02;
const C_TRIANGLE = 0x03;
const C_BOX_FULL = 0x04;
const C_CIRCLE_FULL = 0x05;
const C_SET_WINDOW = 0x06;
const C_SET_MODE = 0x07;
const C_POINT = 0x08;
const C_CLS = 0x09;
const C_SET_3D_PARAMETER = 0x0b;
const C_SET_3D_DATA = 0x0c;
const C_TRANSLATE_3D_TO_2D = 0x0d;
const C_DISPLAY_2D = 0x0e;
const C_DONE = 0x0f;
const C_COLOR = 0x10;
const C_CRT = 0x11;
const C_INIT = 0x12;
const C_AUTO = 0x13;
const C_APAGE = 0x14;
const C_DEPTH = 0x15;

// 3D Parameters.
const P_CX = 0;
const P_CY = 1;
const P_CZ = 2;
const P_DX = 3;
const P_DY = 4;
const P_DZ = 5;
const P_HEAD = 6;
const P_PITCH = 7;
const P_BANK = 8;

/**
 * Decodes an unsigned 16-bit number in big endian.
 * @param {Uint8Array} memory memory image
 * @param {Number} addr memory address
 * @param {Number} read value
 */
const mem_read_u16be = function (memory, addr) {
  return (memory[addr] << 8) | memory[addr + 1];
}

/**
 * Decodes a signed 16-bit number in big endian.
 * @param {Uint8Array} memory memory image
 * @param {Number} addr memory address
 * @param {Number} read value
 */
const mem_read_s16be = function (memory, addr) {
  const u16be = mem_read_u16be(memory, addr);
  if (u16be < 0x8000)
    return u16be;
  return u16be - 0x10000;
}

class Magic2 {
  // constructor
  // @param {CanvasRenderingContext2D} context
  constructor (context) {
    this[_] = {
      // private members
      window: {
        x: 0,
        y: 0,
        w: 0,
        h: 0
      },
      depth: {
        minz: 50,
        maxz: 2000,
      },
      color: false,
      parameters: [0, 0, 0, 0, 0, 0, 0, 0, 0],
      data: {
        pct: 0,
        vertices: new Int16Array(8192 * 3),
        lct: 0,
        indices: new Uint16Array(8192 * 2),
        color: 0
      },
      translate: {
        vertices: new Float32Array(8192 * 3),
        indices: new Float32Array(8192 * 2)
      },
      context: context,
      use2d: context.constructor.name == 'CanvasRenderingContext2D',
      offscreen: null
    };

    if (this[_].use2d) {
      const c = this[_].context;

      // Setup offscreen canvas.
      const canvas = document.createElement('canvas');
      canvas.width = c.canvas.width;
      canvas.height = c.canvas.height;
      this[_].offscreen = canvas.getContext('2d');
      this[_].offscreen.fillStyle = 'rgba(0, 0, 0, 255)';
      this[_].offscreen.fillRect(
          0, 0, canvas.width - 1, canvas.height - 1);

      // Clear onscreen canvas.
      c.fillStyle = 'rgba(0, 0, 0, 255)';
      c.fillRect(0, 0, c.canvas.width - 1, c.canvas.height - 1);
    }
  }

  setWindow (x1, y1, x2, y2) {
    this[_].window.x = x1;
    this[_].window.y = y1;
    this[_].window.w = x2 - x1;
    this[_].window.h = y2 - y1;
  }

  set3dParameter (num, data) {
    this[_].parameters[num] = data;
  }

  set3dData (pct, vertices, lct, indices, color) {
    this[_].data.pct = pct;
    this[_].data.vertices = vertices;
    this[_].data.lct = lct;
    this[_].data.indices = indices;
    this[_].data.color = color;
  }

  set3dRawData (memory, addr) {
    const base = addr;
    this[_].data.pct = mem_read_u16be(memory, addr);
    addr += 2;
    for (let i = 0; i < this[_].data.pct; ++i) {
      this[_].data.vertices[i * 3 + 0] = mem_read_s16be(memory, addr + 0);
      this[_].data.vertices[i * 3 + 1] = mem_read_s16be(memory, addr + 2);
      this[_].data.vertices[i * 3 + 2] = mem_read_s16be(memory, addr + 4);
      addr += 6;
    }
    this[_].data.lct = mem_read_u16be(memory, addr);
    addr += 2;
    if (this[_].color) {
      this[_].data.color = mem_read_u16be(memory, addr);
      addr += 2;
    }
    for (let i = 0; i < this[_].data.lct; ++i) {
      this[_].data.indices[i * 2 + 0] = mem_read_u16be(memory, addr + 0);
      this[_].data.indices[i * 2 + 1] = mem_read_u16be(memory, addr + 2);
      addr += 4;
    }
    return addr - base;
  }

  translate3dTo2d () {
    if (this[_].use2d) {
      // FIXME: Everything other than 'i' should be const and 'i' should be let.
      // Fix them once v8 support optimizing const and let values.

      // Rotate and translate
      var src = this[_].data.vertices;
      var pctx3 = this[_].data.pct * 3;
      var vertices = this[_].translate.vertices;
      var cx = this[_].parameters[P_CX];
      var cy = this[_].parameters[P_CY];
      var cz = this[_].parameters[P_CZ];
      var dx = this[_].parameters[P_DX];
      var dy = this[_].parameters[P_DY];
      var dz = this[_].parameters[P_DZ];
      var rh = this[_].parameters[P_HEAD] / 180 * Math.PI;
      var rp = this[_].parameters[P_PITCH] / 180 * Math.PI;
      var rb = this[_].parameters[P_BANK] / 180 * Math.PI;
      var ch = Math.cos(rh);
      var cp = Math.cos(rp);
      var cb = Math.cos(rb);
      var sh = Math.sin(rh);
      var sp = Math.sin(rp);
      var sb = Math.sin(rb);
      var m11 = sh * sp * sb + ch * cb;
      var m12 = sb * cp;
      var m13 = ch * sp * sb - sh * cb;
      var m14 = dx + cx;
      var m21 = sh * sp * cb - sb * ch;
      var m22 = cp * cb;
      var m23 = ch * sp * cb + sh * sb;
      var m24 = dy + cy;
      var m31 = sh * cp;
      var m32 = -sp;
      var m33 = ch * cp;
      var m34 = dz + cz + this[_].depth.minz;
      var i;
      for (i = 0; i < pctx3; i += 3) {
        var x = src[i + 0] - dx;
        var y = src[i + 1] - dy;
        var z = src[i + 2] - dz;
        vertices[i + 0] = m11 * x + m12 * y + m13 * z + m14;
        vertices[i + 1] = m21 * x + m22 * y + m23 * z + m24;
        vertices[i + 2] = m31 * x + m32 * y + m33 * z + m34;
      }
      // Perspective
      for (i = 0; i < pctx3; i += 3) {
        var nz = vertices[i + 2];
        if (nz <= 0 || this[_].depth.maxz < nz)
          continue;
        var d = nz / 256;
        vertices[i + 0] /= d;
        vertices[i + 1] /= d;
      }
      // Draw
      var indices = this[_].data.indices;
      var lctx2 = this[_].data.lct * 2;
      var c = this[_].offscreen;
      c.beginPath();
      // FIXME: Use a specified color.
      c.strokeStyle = 'rgba(255, 255, 255, 255)';
      var w = c.canvas.width;
      var h = c.canvas.height;
      var ox = w / 2;
      var oy = h / 2;
      // FIXME: Use window information
      var zx = w / 256;
      var zy = h / 256;
      for (i = 0; i < lctx2; i += 2) {
        var s = indices[i + 0] * 3;
        var e = indices[i + 1] * 3;
        var sz = vertices[s + 2];
        var ez = vertices[e + 2];
        if (sz <= 0 || this[_].depth.maxz < sz ||
            ez <= 0 || this[_].depth.maxz < ez) {
          continue;
        }
        c.moveTo(ox + vertices[s + 0] * zx, oy + vertices[s + 1] * zy);
        c.lineTo(ox + vertices[e + 0] * zx, oy + vertices[e + 1] * zy);
      }
      c.closePath();
      c.stroke();
    }
  }

  display2d () {
    if (this[_].use2d) {
      const c = this[_].offscreen;
      const w = c.canvas.width;
      const h = c.canvas.height;
      this[_].context.putImageData(c.getImageData(0, 0, w, h), 0, 0);
      c.fillRect(0, 0, w - 1, h - 1);
    }
  }

  /**
   * Executes magic commands stored in the specified memory.
   * @param {Uint8Array} memory image
   * @param {Number} addr memory address
   * @return {Boolean} true if canvas is updated and it requires waiting vsync
   */
  auto (memory, addr) {
    let shown = false;
    for (;;) {
      var cmd = mem_read_u16be(memory, addr);
      addr += 2;
      switch (cmd) {
        case C_LINE:
        case C_SPLINE:
        case C_BOX:
        case C_TRIANGLE:
        case C_BOX_FULL:
        case C_CIRCLE_FULL:
          throw new Error('magic2: unsupported command ' + cmd);
        case C_SET_WINDOW:
          const x1 = mem_read_u16be(memory, addr + 0);
          const y1 = mem_read_u16be(memory, addr + 2);
          const x2 = mem_read_u16be(memory, addr + 4);
          const y2 = mem_read_u16be(memory, addr + 6);
          addr += 8;
          this.setWindow(x1, y1, x2, y2);
          break;
        case C_SET_MODE:
          const mode = mem_read_u16be(memory, addr);
          addr += 2;
          console.warn('magic2: ignoring command SET_MODE, ' + mode);
          break;
        case C_POINT:
          throw new Error('magic2: unsupported command POINT');
        case C_CLS:
          console.warn('magic2: ignoring command CLS');
          break;
        case C_SET_3D_PARAMETER:
          const param_num = mem_read_u16be(memory, addr + 0);
          const param_val = mem_read_u16be(memory, addr + 2);
          addr += 4;
          this.set3dParameter (param_num, param_val);
          break;
        case C_SET_3D_DATA:
          addr += this.set3dRawData(memory, addr);
          break;
        case C_TRANSLATE_3D_TO_2D:
          this.translate3dTo2d();
          break;
        case C_DISPLAY_2D:
          this.display2d();
          shown = true;
          break;
        case C_DONE:
          return shown;
        case C_COLOR:
          const color = mem_read_u16be(memory, addr);
          addr += 2;
          console.warn('magic2: ignoring command COLOR, ' + color);
          break;
        case C_CRT:
          const crt = mem_read_u16be(memory, addr);
          addr += 2;
          this[_].color = (crt & 0x100) != 0;
          break;
        case C_INIT:
          break;
        case C_AUTO:
          throw new Error('magic2: AUTO should not be used inside AUTO');
        case C_APAGE:
          const apage = mem_read_u16be(memory, addr);
          addr += 2;
          console.warn('magic2: ignoring command APAGE, ' + apage);
          break;
        case C_DEPTH:
          const minz = mem_read_u16be(memory, addr + 0);
          const maxz = mem_read_u16be(memory, addr + 2);
          addr += 4;
          this.depth(minz, maxz);
          break;
        default:
          throw new Error('magic2: unknown command ' + cmd);
      }
    }
    // not reached.
  }

  depth (minz, maxz) {
    this[_].depth.minz = minz;
    this[_].depth.maxz = maxz;
  }
}

global.Magic2 = Magic2;

})(typeof global !== 'undefined' ? global : window);