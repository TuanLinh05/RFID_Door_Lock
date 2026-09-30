# 🔐 RFID Door Lock System

![MCU](https://img.shields.io/badge/MCU-STM32F103C8T6-03234B?logo=stmicroelectronics&logoColor=white)
![RFID](https://img.shields.io/badge/RFID-MFRC522-6A1B9A)
![PCB](https://img.shields.io/badge/PCB-Altium%20Designer-A5915F)
![Language](https://img.shields.io/badge/Language-C-555555?logo=c)

A smart door lock that opens with an **RFID card** or a **keypad password**, built on an **STM32F103C8T6** with a **custom PCB**, an I²C character LCD, voice feedback and an admin menu for managing cards and passwords.

**Course project – Embedded System Design, HCM University of Technology.**
🎬 **Video demo:** [Google Drive](https://drive.google.com/drive/folders/1EJyoBcpR5TI39vi5_LgunK4Ymn7jQt8R?usp=sharing)

---

## ✨ Features

- **Two ways to unlock:** an RFID card (MFRC522, SPI) or a password on the 4×4 keypad.
- **Admin mode:** scan the admin card to change the password, add or remove cards, and view all stored cards.
- **Persistent storage:** the password and card list are saved in the STM32's **internal flash**, so they survive power loss.
- **Brute-force protection:** after 5 wrong attempts the system locks for **5 min**. It locks for **10 min** after 7 attempts and **20 min** after 9.
- **Feedback:** status on the I²C LCD and voice prompts from a **DFPlayer Mini** (granted / denied / locked / admin).
- **Relay output:** drives the electric lock (PB1) and locks again automatically after 7 s.

## 🧠 State machine

```text
IDLE ──(card / key)──▶ INPUT_PASS ──▶ ACCESS_GRANTED ──(timeout)──▶ IDLE
  │                        │
  │                        └──▶ ACCESS_DENIED ──(≥5 fails)──▶ LOCKED ──▶ IDLE
  └──(admin card)──▶ ADMIN_MENU ──▶ SET_PASS_1 → SET_PASS_2
                                 ├─▶ EDIT_CARD (add / delete)
                                 └─▶ VIEW_ALL
```

## 📂 Project structure

```text
RFID_Door_Lock/
├── Firmware/RFID_Door_Lock/   # STM32CubeIDE project
│   └── Core/
│       ├── Src/               # main.c (state machine) + CubeMX peripheral init
│       └── Lib/               # RC522, KEYPAD, LCD I2C, DFPlayer, Storage drivers
├── Hardware/                  # Altium Designer project (latest)
│   └── 01_Design/             # Schematics: MCU, POWER, RFID, LCD, Relay + PCB
├── BTL_HW_V1.0.0/             # Hardware release V1.0.0: design, 3D models, references
└── Docs/                      # Technical requirements document (PDF / DOCX)
```

## 🚀 Getting started

1. Open `Firmware/RFID_Door_Lock` in **STM32CubeIDE**, build and flash it with an ST-Link.
2. Set your admin card UID in `Core/Src/main.c`:
   ```c
   uint8_t ADMIN_UID[4] = {51, 194, 201, 27};   // replace with your card's UID
   ```
3. Copy 4 voice tracks (`0001.mp3` … `0004.mp3`: granted, denied, locked, admin) to the DFPlayer's micro-SD card.
4. Open `Hardware/01_Design/BTL_HW_V1.0.0.PrjPcb` in **Altium Designer** to view or edit the PCB.

---

<p align="center">Made by <a href="https://github.com/TuanLinh05">Vu Tuan Linh</a> · HCMUT</p>
