# School Management System

## Overview

The **School Management System** is a desktop application built with Qt and C++ for schools to manage student data, grades, teacher assignments, and timetables. It provides a graphical interface for easy navigation and data management, utilizing SQLite for persistent storage.

## Features

- **Student Management**
  - Register new students, including personal and academic details
  - Display all students in a table view
  - Export student lists to CSV
  
- **Grade Management**
  - Input and update grades for multiple subjects per student
  - Calculate and display totals and averages
  - View grades by semester and subject

- **Teacher and Timetable Management**
  - Assign teachers to subjects and classrooms
  - Set teacher availability by day and timeslot
  - Generate weekly conflict-free timetables
  - Display generated timetables in a table

- **Other Functionalities**
  - Dashboard summary (e.g., total student count)
  - Data export and import capabilities

## Project Structure

- `dash_board.cpp/h`: Student data management and dashboard UI logic
- `generate.cpp/h`: Teacher assignment, timetable generation, and associated UI
- `roster.cpp/h`: Grade management and grade roster display
- Additional modules: Registration screen, payment management, calendar, etc.

## How to Build

1. **Prerequisites**
   - Qt 5.x or 6.x (Qt Creator recommended)
   - C++ compiler supporting C++11 or later

2. **Build Steps**
   - Clone the repository:
     ```
     git clone https://github.com/Abdibaye/School_Management_System.git
     ```
   - Open the project in Qt Creator (`School_Management_System.pro`)
   - Configure the kit and build the project
   - Run the application from Qt Creator

## Usage

- **Add Students:** Use the registration screen to add new students.
- **View/Edit Students:** Access the dashboard to see and update student records.
- **Manage Grades:** Open the grade roster to input or edit student grades.
- **Generate Timetables:** Use the timetable generator to create teacher schedules, avoiding conflicts.
- **Export Data:** Export student lists and other data to CSV for backup or reporting.

## Database

- Uses SQLite database `updatestudent.db`
- Tables include `students`, `rosters`, and others as needed
- Database schema is initialized on first run if not present

## Contributing

Contributions are welcome! Please open issues or pull requests for improvements or bug fixes.


*This project was developed by Abdibaye. For questions or support, please open an issue in the repository.*
