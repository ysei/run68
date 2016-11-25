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
})();
