#!/bin/bash

texte=("1" "2" "3" "4. Bit Operations" "5. LED Traffic light" "6. Button control" "7. 7 Segment Display" "8. External Interrupt" "9. External Crystal Oscilattor" "10. UART serial Loopback" "11. UART Interrupt" "12. Analog comperator" "13. Analog Digital Converter" "14. SPI Master 75HC595" "15. SPI Master 74HC165" "16. SPI Slave" "17. Attiny USI SPI Master" "18. EEPROM" "19. I2C Master LM75" "20. I2C Slave" "21. Attiny44 USI I2C Master" "22. Timer" "23. PWM" "24. Watchdog")

day = "$(date '+%d')"
echo ${texte[$day]} >> README.md

git add $today_* README.md
git commit -m "Upload Day $day"
git push

