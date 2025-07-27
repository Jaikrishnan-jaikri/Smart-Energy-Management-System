
# Smart Energy Management System 🔌🏠

A smart IoT-based energy monitoring and control system designed to track real-time power usage, identify energy-hungry appliances, and offer optimization suggestions to reduce electricity bills and environmental impact.

## 🧠 Problem Statement

Household electricity consumption often leads to high utility costs and energy wastage due to inefficient appliance usage and lack of visibility. Users are unaware of which appliances consume the most power and have no way to take immediate corrective action.

## ✅ Solution

This project provides a real-time Smart Energy Monitoring System that continuously monitors power consumption of home appliances, identifies high-energy devices, and recommends ways to optimize usage. It also allows control over certain appliances and sends usage notifications to users via a web platform.

## ⚙️ Implementation

- **Hardware**: ESP32 microcontroller reads input from ACS712 current sensors connected to various appliances.
- **Data Processing**: Raw analog signals are converted to current readings, then used to compute power consumption in real-time.
- **Database**: Data is stored in Firebase Realtime Database with timestamps.
- **Web Dashboard**: Displays categorized energy usage (fans, lights, sockets, etc.) with control switches and usage insights.
- **User Interaction**: Users receive suggestions like turning off idle ACs, replacing inefficient devices, or adjusting usage patterns based on time of day and season.

## 🧩 Hardware Components

- ESP32 Dev Module
- ACS712 Current Sensor
- 5V 30A Relay Module
- AC-DC Power Supply Module
- RTC Module (DS3231) – for timestamping data

## 💻 Software/Tools Used

- Arduino IDE (for ESP32 programming)
- Firebase Realtime Database
- HTML/CSS/JavaScript (for web UI)
- Python (optional, for backend processing)
- GitHub (for version control)

## 🔍 Features

- Real-time power monitoring (W, kWh)
- Appliance-wise power consumption tracking
- Device control (fan, light, AC) via relays
- Alerts and usage-based suggestions
- Firebase integration for cloud logging
- Web dashboard for live monitoring
- Device renaming (e.g., SOC-1 → Fridge)

## 📊 Data Logging

Firebase stores energy usage with timestamps. Logs are segmented hourly and daily. Users can analyze the historical data and receive energy-saving suggestions.




