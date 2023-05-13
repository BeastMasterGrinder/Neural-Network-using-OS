
-- Create the Roles table
CREATE TABLE Roles (
  RoleID  INT  PRIMARY KEY  IDENTITY(1,1) ,
  RoleName VARCHAR(255)
);

-- Create the Semester table
CREATE TABLE Semester (
SemesterID INT PRIMARY KEY  IDENTITY(1,1),
SemesterName VARCHAR(255)
);

-- Create the Users table
CREATE TABLE Users (
  UserID INT PRIMARY KEY  IDENTITY(1,1),
  Username VARCHAR(255),
  Password VARCHAR(255),
  Email VARCHAR(255),
  Phone VARCHAR(255),
  Address VARCHAR(255),
  RoleID INT,
  FOREIGN KEY (RoleID) REFERENCES Roles(RoleID)
);
use nig4
select * from users
select * from roles

-- Create the Coordinator table
CREATE TABLE Coordinator (
  UserID INT PRIMARY KEY,
  FOREIGN KEY (UserID) REFERENCES Users(UserID)
);
SELECT C.CourseCode, C.CourseName
FROM Courses C
INNER JOIN CourseAllocation CA ON CA.CourseID = C.CourseID
INNER JOIN Faculty F ON F.FacultyID = CA.FacultyID
INNER JOIN Users U ON U.UserID = F.UserID
WHERE U.Username = 'faculty1' AND U.Password = 'faculty123';

-- Create the Students table
CREATE TABLE Students (
  StudentID INT PRIMARY KEY  IDENTITY(1,1) ,
  UserID INT,
  Name VARCHAR(255),
  RegistrationNumber VARCHAR(255),
  CGPA DECIMAL(4, 2),
  SGPA DECIMAL(4, 2),
  Batch INT,
  CurrentSemester INT,
  FOREIGN KEY (UserID) REFERENCES Users(UserID)
);     

-- Create the Faculty table
CREATE TABLE Faculty (
  FacultyID INT PRIMARY KEY  IDENTITY(1,1),
  UserID INT,
  Name VARCHAR(255),
  Department VARCHAR(255),
  FOREIGN KEY (UserID) REFERENCES Users(UserID)
);

-- Create the Courses table
CREATE TABLE Courses (
  CourseID INT PRIMARY KEY  IDENTITY(1,1),
  CourseCode VARCHAR(255),
  CourseName VARCHAR(255),
  CreditHours INT,
  Semester INT
);

-- Create the Sections table
CREATE TABLE Sections (
  SectionID INT PRIMARY KEY  IDENTITY(1,1),
  CourseID INT,
  SemesterID INT,
  SectionName VARCHAR(255),
  MaximumCapacity INT,
  FacultyID INT,
  FOREIGN KEY (CourseID) REFERENCES Courses(CourseID),
  FOREIGN KEY (SemesterID) REFERENCES Semester(SemesterID),
  FOREIGN KEY (FacultyID) REFERENCES Faculty(FacultyID)
);

-- Create the CourseInstructors table
CREATE TABLE CourseInstructors (
  CourseInstructorID INT PRIMARY KEY  IDENTITY(1,1),
  FacultyID INT,
  SectionID INT,
  FOREIGN KEY (FacultyID) REFERENCES Faculty(FacultyID),
  FOREIGN KEY (SectionID) REFERENCES Sections(SectionID)
);

-- Create the CourseLabInstructors table
CREATE TABLE CourseLabInstructors (
  CourseLabInstructorID INT PRIMARY KEY  IDENTITY(1,1),
  FacultyID INT,
  SectionID INT,
  FOREIGN KEY (FacultyID) REFERENCES Faculty(FacultyID),
  FOREIGN KEY (SectionID) REFERENCES Sections(SectionID)
);

-- Create the StudentsSections table
CREATE TABLE StudentsSections (
  StudentSectionID INT PRIMARY KEY  IDENTITY(1,1),
  StudentID INT,
  SectionID INT,
  FOREIGN KEY (StudentID) REFERENCES Students(StudentID),
  FOREIGN KEY (SectionID) REFERENCES Sections(SectionID)
);

-- Create the EvaluationTypes table
CREATE TABLE EvaluationTypes (
  EvaluationTypeID INT PRIMARY KEY  IDENTITY(1,1),
  EvaluationTypeName VARCHAR(255)
);

-- Create the EvaluationMarks table
CREATE TABLE EvaluationMarks (
  EvaluationMarkID INT PRIMARY KEY  IDENTITY(1,1),
  StudentSectionID INT,
  EvaluationTypeID INT,
  MarksObtained DECIMAL(5, 2),
  FOREIGN KEY (StudentSectionID) REFERENCES StudentsSections(StudentSectionID),
  FOREIGN KEY (EvaluationTypeID) REFERENCES EvaluationTypes(EvaluationTypeID)
);

-- Create the Attendance table
CREATE TABLE Attendance (
  AttendanceID INT PRIMARY KEY  IDENTITY(1,1),
  StudentSectionID INT,
  Session VARCHAR(255),
  Date DATE,
  IsPresent TINYINT,
  FOREIGN KEY (StudentSectionID) REFERENCES StudentsSections(StudentSectionID)
);

-- Create the Feedback table
CREATE TABLE Feedback (
  FeedbackID INT PRIMARY KEY  IDENTITY(1,1),
  StudentID INT,
  FacultyID INT,
  CourseID INT,
  FeedbackText VARCHAR(255),
FOREIGN KEY (StudentID) REFERENCES Students(StudentID),
FOREIGN KEY (FacultyID) REFERENCES Faculty(FacultyID),
FOREIGN KEY (CourseID) REFERENCES Courses(CourseID)
);

-- Create the Grades table
CREATE TABLE Grades (
GradeID INT PRIMARY KEY  IDENTITY(1,1),
GradeName VARCHAR(255)
);

-- Create the GradeReport table
CREATE TABLE GradeReport (
GradeReportID INT PRIMARY KEY  IDENTITY(1,1),
StudentID INT,
SectionID INT,
GradeID INT,
FOREIGN KEY (StudentID) REFERENCES Students(StudentID),
FOREIGN KEY (SectionID) REFERENCES Sections(SectionID),
FOREIGN KEY (GradeID) REFERENCES Grades(GradeID)
);

-- Create the AttendanceSheet table
CREATE TABLE AttendanceSheet (
AttendanceSheetID INT PRIMARY KEY  IDENTITY(1,1),
SectionID INT,
Date DATE,
IsPresent TINYINT,
FOREIGN KEY (SectionID) REFERENCES Sections(SectionID)
);

-- Create the CourseAllocation table
CREATE TABLE CourseAllocation (
CourseAllocationID INT PRIMARY KEY  IDENTITY(1,1) ,
CourseID INT,
FacultyID INT,
CoordinatorID INT,
FOREIGN KEY (CourseID) REFERENCES Courses(CourseID),
FOREIGN KEY (FacultyID) REFERENCES Faculty(FacultyID),
FOREIGN KEY (CoordinatorID) REFERENCES Coordinator(UserID)
);

-- Create the EvaluationReport table
CREATE TABLE EvaluationReport (
EvaluationReportID INT PRIMARY KEY  IDENTITY(1,1),
SectionID INT,
EvaluationTypeID INT,
TotalMarks DECIMAL(5, 2),
FOREIGN KEY (SectionID) REFERENCES Sections(SectionID),
FOREIGN KEY (EvaluationTypeID) REFERENCES EvaluationTypes(EvaluationTypeID)
);

-- Create the GradesCount table
CREATE TABLE GradesCount (
GradesCountID INT PRIMARY KEY  IDENTITY(1,1),
SectionID INT,
GradeID INT,
Count INT,
FOREIGN KEY (SectionID) REFERENCES Sections(SectionID),
FOREIGN KEY (GradeID) REFERENCES Grades(GradeID)
);






-- inserting data
INSERT INTO Roles (RoleName) VALUES ('Faculty');
INSERT INTO Roles (RoleName) VALUES ('Student');
INSERT INTO Roles (RoleName) VALUES ('Admin');


-- SEMESTER
INSERT INTO Semester (SemesterName) VALUES ('Fall 2019');
INSERT INTO Semester (SemesterName) VALUES ('Spring 2020');
INSERT INTO Semester (SemesterName) VALUES ('Fall 2020');
INSERT INTO Semester (SemesterName) VALUES ('Spring 2021');
INSERT INTO Semester (SemesterName) VALUES ('Fall 2021');
INSERT INTO Semester (SemesterName) VALUES ('Spring 2022');
INSERT INTO Semester (SemesterName) VALUES ('Fall 2022');
INSERT INTO Semester (SemesterName) VALUES ('Spring 2023');

-- USERS

-- Insert data into the Users table for academic role
INSERT INTO Users (Username, Password, Email, Phone, Address, RoleID)
VALUES 
('academic_user2', 'password123', 'academic_user2@example.com', '1234567890', 'Academic Address 2', 3),
('academic_user3', 'password123', 'academic_user3@example.com', '1234567890', 'Academic Address 3', 3),
('academic_user4', 'password123', 'academic_user4@example.com', '1234567890', 'Academic Address 4', 3),
('academic_user5', 'password123', 'academic_user5@example.com', '1234567890', 'Academic Address 5', 3),
('academic_user6', 'password123', 'academic_user6@example.com', '1234567890', 'Academic Address 6', 3),
('academic_user7', 'password123', 'academic_user7@example.com', '1234567890', 'Academic Address 7', 3),
('academic_user8', 'password123', 'academic_user8@example.com', '1234567890', 'Academic Address 8', 3),
('academic_user9', 'password123', 'academic_user9@example.com', '1234567890', 'Academic Address 9', 3),
('academic_user10', 'password123', 'academic_user10@example.com', '1234567890', 'Academic Address 10', 3);

-- Insert data into the Users table for student role
INSERT INTO Users (Username, Password, Email, Phone, Address, RoleID)
VALUES 
('student_user2', 'password123', 'student_user2@example.com', '1234567890', 'Student Address 2', 2),
('student_user3', 'password123', 'student_user3@example.com', '1234567890', 'Student Address 3', 2),
('student_user4', 'password123', 'student_user4@example.com', '1234567890', 'Student Address 4', 2),
('student_user5', 'password123', 'student_user5@example.com', '1234567890', 'Student Address 5', 2),
('student_user6', 'password123', 'student_user6@example.com', '1234567890', 'Student Address 6', 2),
('student_user7', 'password123', 'student_user7@example.com', '1234567890', 'Student Address 7', 2),
('student_user8', 'password123', 'student_user8@example.com', '1234567890', 'Student Address 8', 2),
('student_user9', 'password123', 'student_user9@example.com', '1234567890', 'Student Address 9', 2),
('student_user10', 'password123', 'student_user10@example.com', '1234567890', 'Student Address 10', 2)

-- Insert data into the Users table for faculty role

-- Insert data into the Users table for faculty role
INSERT INTO Users (Username, Password, Email, Phone, Address, RoleID)
VALUES 
('faculty_user2', 'password123', 'faculty_user2@example.com', '1234567890', 'Faculty Address 2', 1),
('faculty_user3', 'password123', 'faculty_user3@example.com', '1234567890', 'Faculty Address 3', 1),
('faculty_user4', 'password123', 'faculty_user4@example.com', '1234567890', 'Faculty Address 4', 1),
('faculty_user5', 'password123', 'faculty_user5@example.com', '1234567890', 'Faculty Address 5', 1),
('faculty_user6', 'password123', 'faculty_user6@example.com', '1234567890', 'Faculty Address 6', 1),
('faculty_user7', 'password123', 'faculty_user7@example.com', '1234567890', 'Faculty Address 7', 1),
('faculty_user8', 'password123', 'faculty_user8@example.com', '1234567890', 'Faculty Address 8', 1),
('faculty_user9', 'password123', 'faculty_user9@example.com', '1234567890', 'Faculty Address 9', 1),
('faculty_user10', 'password123', 'faculty_user10@example.com', '1234567890', 'Faculty Address 10', 1);


-- COORDINATOR
INSERT INTO Coordinator (UserID) VALUES (1);
INSERT INTO Coordinator (UserID) VALUES (2);
INSERT INTO Coordinator (UserID) VALUES (3);


-- STUDENTS
INSERT INTO Students (UserID, Name, RegistrationNumber, CGPA, SGPA, Batch, CurrentSemester)
