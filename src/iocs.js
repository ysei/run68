/* global magic2 */
/* global navigator */
(function() {
  var keyStates = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

  // ESC, F1-F5
  window.addEventListener('keydown', function(e) {
    switch (e.code) {
      case "ArrowDown":
        keyStates[7] |= (1 << 6);
        keyStates[9] |= (1 << 4);  // Num 2
        break;
      case "ArrowLeft":
        keyStates[7] |= (1 << 3);
        keyStates[8] |= (1 << 7);  // Num 4
        break;
      case "ArrowRight":
        keyStates[7] |= (1 << 5);
        keyStates[9] |= (1 << 1);  // Num 6
        break;
      case "ArrowUp":
        keyStates[7] |= (1 << 4);
        keyStates[8] |= (1 << 4);  // Num 8
        break;
      case "Escape":
        keyStates[0] |= (1 << 1);
        break;
      case "F1":
      case "Digit1":
        keyStates[12] |= (1 << 3);
        break;
      case "F2":
      case "Digit2":
        keyStates[12] |= (1 << 4);
        break;
      case "F3":
      case "Digit3":
        keyStates[12] |= (1 << 5);
        break;
      case "F4":
      case "Digit4":
        keyStates[12] |= (1 << 6);
        break;
      case "F5":
      case "Digit5":
        keyStates[12] |= (1 << 7);
        break;
      case "KeyX":
        keyStates[5] |= (1 << 3);
        break;
      case "KeyZ":
        keyStates[5] |= (1 << 2);
        break;
      default:
        console.log(e);
        break;
    }
  }, false);

  window.addEventListener('keyup', function(e) {
    switch (e.code) {
      case "ArrowDown":
        keyStates[7] &= ~(1 << 6);
        keyStates[9] &= ~(1 << 4);  // Num 2
        break;
      case "ArrowLeft":
        keyStates[7] &= ~(1 << 3);
        keyStates[8] &= ~(1 << 7);  // Num 4
        break;
      case "ArrowRight":
        keyStates[7] &= ~(1 << 5);
        keyStates[9] &= ~(1 << 1);  // Num 6
        break;
      case "ArrowUp":
        keyStates[7] &= ~(1 << 4);
        keyStates[8] &= ~(1 << 4);  // Num 8
        break;
      case "Escape":
        keyStates[0] &= ~(1 << 1);
        break;
      case "F1":
      case "Digit1":
        keyStates[12] &= ~(1 << 3);
        break;
      case "F2":
      case "Digit2":
        keyStates[12] &= ~(1 << 4);
        break;
      case "F3":
      case "Digit3":
        keyStates[12] &= ~(1 << 5);
        break;
      case "F4":
      case "Digit4":
        keyStates[12] &= ~(1 << 6);
        break;
      case "F5":
      case "Digit5":
        keyStates[12] &= ~(1 << 7);
        break;
      case "KeyX":
        keyStates[5] &= ~(1 << 3);
        break;
      case "KeyZ":
        keyStates[5] &= ~(1 << 2);
        break;
      default:
        console.log(e);
        break;
    }
  }, false);

  window.iocs_bitsns = function(group) {
    return keyStates[group];
  };
 
  window.iocs_joyget = function (id) {
    if (!navigator.getGamepads)
      return 0xff;
    var pad = navigator.getGamepads()[id];
    if (!pad)
      return 0xff;
    var bits = 0xff;
    var x = pad.axes[0] || 0;
    var y = pad.axes[1] || 0;
    var a = pad.buttons[0] && pad.buttons[0].pressed;
    var b = pad.buttons[1] && pad.buttons[1].pressed;
    if (x < -0.5)
      bits &= ~0x04;
    else if (x > 0.5)
      bits &= ~0x08;
    if (y < -0.5)
      bits &= ~0x01;
    else if (y > 0.5)
      bits &= ~0x02;
    if (a)
      bits &= ~0x40;
    if (b)
      bits &= ~0x20;
    return bits;
  };

  var sprites = new Array(128);
  for (var i = 0; i < 128; ++i) {
    sprites[i] = {
      x: 0,
      y: 0,
      id: 0,
      flipX: false,
      flipY: false,
      show: false,
      tick: 0
    };
  }
  var sprite = false;

  magic2.vsync(function(c) {
    if (!sprite)
      return;
    var scaleWideX = c.canvas.width / 256;
    var scaleX = c.canvas.height / 256 * 4 / 3;
    var scaleY = c.canvas.height / 256;
    var offsetX = (c.canvas.width - c.canvas.height * 4 / 3) / 2;
    for (var i = 0; i < 128; ++i) {
      var s = sprites[i];
      if (!s.show)
        continue;
      var x = s.x - 16;
      var y = s.y - 16;
      switch (s.id) {
        case 0:  // 2x2 star
        case 1:  // 1x1 star
          c.fillStyle = 'rgba(255, 255, 255, 1.0)';
          c.fillRect(
              (x + 4) * scaleWideX,
              (y + 4) * scaleY,
              s.id ? 1 : 2,
              s.id ? 1 : 2);
          continue;
        case 7: {  // scope
          var scale = 1 + (s.tick++ % 10) / 10;
          var w = 16 * scaleX / scale;
          var h = 16 * scaleY / scale;
          var bx = x * scaleX + offsetX + (16 * scaleX - w) / 2;
          var by = y * scaleY + (16 * scaleY - h) / 2;
          c.strokeStyle = 'rgba(0, 255, 255, 0.7)';
          c.beginPath();
          c.moveTo(bx, by + h / 2);
          c.lineTo(bx, by + h / 5);
          c.lineTo(bx + w / 5, by);
          c.lineTo(bx + w / 2, by);
          c.moveTo(bx + w, by + h / 2);
          c.lineTo(bx + w, by + h * 4 / 5);
          c.lineTo(bx + w * 4 / 5, by + h);
          c.lineTo(bx + w / 2, by + h);
          c.moveTo(bx + w / 4, by + h / 2);
          c.lineTo(bx + w / 3, by + h * 2 / 3);
          c.lineTo(bx + w * 2 / 3, by + h * 2 / 3);
          c.lineTo(bx + w * 3 / 4, by + h / 2);
          c.moveTo(bx + w * 2 / 5, by + h / 2);
          c.lineTo(bx + w * 4 / 11, by + h * 4 / 7);
          c.moveTo(bx + w * 3 / 5, by + h / 2);
          c.lineTo(bx + w * 7 / 11, by + h * 4 / 7);
          c.moveTo(bx + w * 4 / 16, by + h * 12 / 16);
          c.lineTo(bx + w * 3 / 16, by + h * 13 / 16);
          c.moveTo(bx + w * 2 / 7, by + h / 3);
          c.lineTo(bx + w * 5 / 7, by + h / 3);
          c.stroke();
          c.closePath();
          c.strokeStyle = 'rgba(255, 0, 0, 0.7)';
          c.beginPath();
          c.moveTo(bx + w * 12 / 16, by + h * 12 / 16);
          c.lineTo(bx + w * 13 / 16, by + h * 13 / 16);
          c.stroke();
          c.closePath();
          continue; }
        default:
          console.warn('sprite ' + s.id + ' is not supported, showing a TOFU.')
        break;
      }
      // Default: TOFU.
      c.fillStyle = 'rgba(255, 255, 255, 1.0)';
      c.fillRect(
          x * scaleX + offsetX,
          y * scaleY,
          16 * scaleX,
          16 * scaleY);
    }
  });

  window.iocs_sp_on = function() {
    sprite = true;
  };

  window.iocs_sp_off = function() {
    sprite = false;
  };
 
  window.iocs_sp_regst = function(id, x, y, code, prio) {
    var sprite = sprites[id & 0x1f];
    sprite.x = x & 0x3ff;
    sprite.y = y & 0x3ff;
    sprite.id = code & 0xff;
    sprite.flipX = (code & 0x4000) != 0;
    sprite.flipY = (code & 0x8000) != 0;
    sprite.show = prio != 0;
  };

  window.io_sprite_data = function(index, data) {
    var id = index >> 2;
    var sprite = sprites[id];
    switch (index & 3) {
      case 0:  // x
        sprite.x = data & 0x3ff;
        break;
      case 1:  // y
        sprite.y = data & 0x3ff;
        break;
      case 2:  // code
        sprite.id = data & 0xff;
        sprite.flipX = (data & 0x4000) != 0;
        sprite.flipY = (data & 0x8000) != 0;
        break;
      case 3:  // prio
        if (!sprite.show && data != 0)
          sprite.tick = 0;
        sprite.show = data != 0;
        break;
    }
  };
})();
