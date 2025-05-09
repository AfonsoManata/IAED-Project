# IAED-Project
# Vaccine Management System - IAED 2024/2025 💉📅

Project developed for the Introduction to Algorithms and Data Structures (IAED) course at Instituto Superior Técnico I got 20/20 in this project.

## 📋 Description
A C-based vaccine management system that allows:
- Registering vaccine batches with expiration dates and available doses
- Managing vaccine administrations to patients
- Tracking expiration dates and availability
- Generating organized reports

## 🚀 Key Features

| Command | Description                             |
|---------|-----------------------------------------|
| `q`     | Terminates the program                  |
| `c`     | Adds a new vaccine batch                |
| `l`     | Lists available vaccines                |
| `a`     | Administers a vaccine dose to a patient |
| `r`     | Removes vaccine availability            |
| `d`     | Deletes vaccination records             |
| `u`     | Lists patient vaccinations              |
| `t`     | Advances simulated time                 |

## 🛠️ Technical Specifications
- **Max batches**: 1000
- **Vaccine name**: ≤50 bytes (UTF-8), no whitespace
- **Batch ID**: ≤20 hex chars (0-9, A-F)
- **Patient name**: ≤200 bytes (quotes for spaces)
- **Dynamic memory allocation** with error handling
- **No global variables** allowed

## ⚙️ Compilation & Testing
```bash
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c
