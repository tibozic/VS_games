# Gaming Console with Asteroids Game on STM32H750  
This project implements a simple gaming console based on the STM32H750 microcontroller. <br>  
The console includes the popular arcade game Asteroids and allows players' scores to be recorded through RFID authentication.

## Console Features:  
1. **Asteroids Game Adaptation:**  
   - A classic game where the player controls a spaceship and destroys asteroids.  
   - The game uses graphics and a touchscreen display.  

2. **Player Authentication with RFID:**  
   - Each player can use an RFID card to log in. The card contains only the player's ID, while other data is stored on the console.  
   - Players who do not log in can play as Guests, but their score is not tied to any profile.

3. **Score Storage:**  
   - The console saves player profiles and their achieved scores locally on the device.  
   - Scores are reset upon restarting the console.

## Technologies  
- C  
- STM32  
- FreeRTOS  
- TouchGFX  
- RFID  
- SPI  
- UART  
- Timers  
- Tasks  
- Semaphores

## Possible Improvements  
- More games.  
- Persistent score storage.  
- Storing player scores on the card.
