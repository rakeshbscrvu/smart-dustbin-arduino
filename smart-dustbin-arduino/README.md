# Smart Auto-Opening Dustbin with Bin-Full Phone Alert using Arduino

An Arduino Uno based IoT project. The dustbin lid opens by itself when someone comes near (no touch needed), and the cleaning person gets an alert on their phone **only when the bin is full**.

## 1. Problem Statement

Public and household dustbins are touched by many people, which spreads germs. They are also usually emptied on a fixed schedule instead of when they are actually full. Because nobody can tell the fill level without opening the bin, it often overflows, causing bad smell, pests and an unhygienic surrounding. This system makes the bin touch-free and tells the cleaning person on their phone when the bin is full, so it is emptied only when needed.

## 2. Objectives

- Open the lid automatically, without touching, when a person comes near the bin.
- Close the lid automatically after 3 seconds.
- Measure the fill level inside the bin continuously using an ultrasonic sensor.
- Send an alert to the user's phone **only when the bin is full** (80% or more).
- Give a local warning (red LED and buzzer) when the bin is full, and a green LED when it is not.

## 3. Components Required

| Type | Component | Qty | Purpose |
|---|---|---|---|
| Controller | Arduino Uno | 1 | Main controller |
| Sensor | HC-SR04 ultrasonic sensor (outer, front of bin) | 1 | Detects a person or hand near the bin |
| Sensor | HC-SR04 ultrasonic sensor (inner, under the lid) | 1 | Measures the fill level |
| Actuator | SG90 micro servo motor | 1 | Opens and closes the lid |
| Actuator | Buzzer | 1 | Sound alarm when full |
| Indicators | Red LED and green LED | 2 | Full / not-full status |
| Communication | HC-05 Bluetooth module | 1 | Sends the alert to the phone |
| Resistors | 220 ohm (x2), 1 kohm (x1), 2 kohm (x1) | 4 | LED protection and HC-05 voltage divider |
| Others | Breadboard, jumper wires, USB cable or 9V battery | | Connections and power |
| Software | Arduino IDE, Tinkercad, Bluetooth terminal app | | Programming, simulation, receiving the alert |

## 4. Circuit Diagram / Connection Plan

Tinkercad design: **PASTE YOUR TINKERCAD SHARE LINK HERE**

Screenshots are in the [`circuit/`](circuit/) folder.

### Power rails on the breadboard
- Arduino **5V** to breadboard **red (+) rail**
- Arduino **GND** to breadboard **blue (-) rail**
- Every component VCC goes to the red rail, every GND goes to the blue rail.

### Pin connections

| Component | Component pin | Connects to |
|---|---|---|
| **Outer ultrasonic sensor** | VCC | 5V rail |
| | TRIG | Arduino D9 |
| | ECHO | Arduino D10 |
| | GND | GND rail |
| **Inner ultrasonic sensor** | VCC | 5V rail |
| | TRIG | Arduino D7 |
| | ECHO | Arduino D8 |
| | GND | GND rail |
| **Servo motor (SG90)** | Red wire | 5V rail |
| | Brown/black wire | GND rail |
| | Orange/yellow (signal) | Arduino D6 |
| **Buzzer** | + (long leg) | Arduino D4 |
| | - | GND rail |
| **Red LED** | Anode (long leg) via 220 ohm | Arduino D3 |
| | Cathode (short leg) | GND rail |
| **Green LED** | Anode (long leg) via 220 ohm | Arduino D2 |
| | Cathode (short leg) | GND rail |
| **HC-05 Bluetooth** | VCC | 5V rail |
| | GND | GND rail |
| | TXD | Arduino D11 (software RX) |
| | RXD | Arduino D12 through a voltage divider (below) |

### Voltage divider for HC-05 RXD
The HC-05 RX pin works at 3.3V but the Arduino sends 5V, so the signal is reduced:

```
Arduino D12 --[ 1 kohm ]--+-- HC-05 RXD
                          |
                        [ 2 kohm ]
                          |
                         GND
```

### Placement on the bin
- **Outer sensor:** front of the lid/bin, facing outward, to detect a person.
- **Inner sensor:** fixed under the lid, facing down toward the garbage.
- **Servo:** attached at the lid hinge to lift the lid.

## Working Logic

1. If the outer sensor detects an object within 20 cm, the servo opens the lid to 90 degrees, waits 3 seconds, then closes it.
2. After the lid is closed, the inner sensor measures the distance to the garbage and converts it to fill %.
3. If the fill is 80% or more, the red LED and buzzer turn on, and `ALERT: Dustbin is FULL!` is sent to the phone **once**.
4. When the bin is emptied (below 50%), the green LED turns on, the buzzer stops and the alert is ready again.

The inner sensor is read only while the lid is closed, so the open lid or a hand does not give a false reading.

## How to Run

1. Open [`smart_dustbin.ino`](smart_dustbin.ino) in the Arduino IDE (or paste it in the Tinkercad **Code > Text** panel).
2. Set `BIN_HEIGHT` to your bin's height in cm (distance from the inner sensor to the bin bottom).
3. Select **Arduino Uno** and the correct port, then upload.
4. Open the Serial Monitor at 9600 baud to see the fill level.
5. On the phone, install **Serial Bluetooth Terminal**, pair with the HC-05 (password usually `1234` or `0000`) and connect. The alert appears there when the bin is full.

**Tinkercad note:** Tinkercad has no HC-05, so in the simulation the alert shows in the Serial Monitor. Drag the sensor's object closer or farther to test open, full and empty.

**Upload tip:** if the upload fails on real hardware, disconnect the HC-05 from D11/D12 while uploading.

## Repository Structure

```
smart-dustbin-arduino/
├── README.md
├── smart_dustbin.ino          Arduino code
├── circuit/
│   ├── tinkercad_circuit.png  screenshot of the circuit
│   └── simulation_output.png  screenshot of the Serial Monitor alert
└── images/
    └── prototype.jpg          photo of the final model
```

## Team

| Name | Role |
|---|---|
| Member 1 | |
| Member 2 | |
| Member 3 | |
| Member 4 | |

All team members have been added as collaborators with write access to this repository.
