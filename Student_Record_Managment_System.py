import pickle
import os

FILE_NAME = "students.dat"

# --------------------- Student Class ----------------------
class Student:
    def __init__(self, roll, name, marks):
        self.roll = roll
        self.name = name
        self.marks = marks
        self.total = sum(marks)
        self.average = self.total / 5
        self.grade = self.calculate_grade()

    def calculate_grade(self):
        if self.average >= 90:
            return 'A'
        elif self.average >= 75:
            return 'B'
        elif self.average >= 60:
            return 'C'
        elif self.average >= 40:
            return 'D'
        else:
            return 'F'


# --------------------- File Operations ----------------------
def load_from_file():
    if not os.path.exists(FILE_NAME):
        return []
    try:
        with open(FILE_NAME, "rb") as f:
            return pickle.load(f)
    except:
        return []


def save_to_file(students):
    with open(FILE_NAME, "wb") as f:
        pickle.dump(students, f)


# --------------------- Add Student ----------------------
def add_student():
    students = load_from_file()

    roll = int(input("Enter Roll Number: "))
    name = input("Enter Student Name: ")

    marks = []
    print("Enter marks of 5 subjects (0-100 only):")
    for i in range(5):
        while True:
            m = float(input(f"Subject {i+1}: "))
            if 0 <= m <= 100:
                marks.append(m)
                break
            else:
                print("Invalid input! Marks must be between 0 and 100.")

    student = Student(roll, name, marks)
    students.append(student)
    save_to_file(students)

    print("\nRecord Added Successfully!\n")


# --------------------- Display All ----------------------
def display_all():
    students = load_from_file()
    if not students:
        print("\nNo records found.\n")
        return

    print("\n===== All Student Records =====")
    for s in students:
        print(f"\nRoll: {s.roll}")
        print(f"Name: {s.name}")
        print(f"Total: {s.total:.2f}")
        print(f"Average: {s.average:.2f}")
        print(f"Grade: {s.grade}")


# --------------------- Binary Search ----------------------
def binary_search():
    students = load_from_file()

    # Sort by roll before binary search
    students.sort(key=lambda x: x.roll)

    roll = int(input("Enter Roll Number to Search: "))
    low, high = 0, len(students) - 1

    while low <= high:
        mid = (low + high) // 2
        if students[mid].roll == roll:
            print("\nRecord Found!")
            print(f"Name: {students[mid].name}")
            print(f"Total: {students[mid].total:.2f}")
            return
        elif roll > students[mid].roll:
            low = mid + 1
        else:
            high = mid - 1

    print("\nRecord Not Found.\n")


# --------------------- Insertion Sort (by Total Descending) ----------------------
def insertion_sort():
    students = load_from_file()

    for i in range(1, len(students)):
        key = students[i]
        j = i - 1
        while j >= 0 and students[j].total < key.total:
            students[j + 1] = students[j]
            j -= 1
        students[j + 1] = key

    print("\nRecords sorted using Insertion Sort (by Total Descending):")
    for s in students:
        print(f"{s.roll} - {s.name} - {s.total:.2f}")


# --------------------- Update Record ----------------------
def update_record():
    students = load_from_file()
    roll = int(input("Enter roll to modify: "))

    for s in students:
        if s.roll == roll:
            s.name = input("Enter new name: ")
            marks = []
            for i in range(5):
                marks.append(float(input(f"Subject {i+1}: ")))

            s.marks = marks
            s.total = sum(marks)
            s.average = s.total / 5
            s.grade = s.calculate_grade()

            save_to_file(students)
            print("\nRecord updated successfully!\n")
            return

    print("\nRecord not found.\n")


# --------------------- Delete Record ----------------------
def delete_record():
    students = load_from_file()
    roll = int(input("Enter roll to delete: "))

    new_students = [s for s in students if s.roll != roll]

    if len(new_students) == len(students):
        print("\nRecord not found.\n")
    else:
        save_to_file(new_students)
        print("\nRecord deleted successfully!\n")


# --------------------- Main Menu ----------------------
def menu():
    while True:
        print("\n============ STUDENT MANAGEMENT SYSTEM ============")
        print("1. Add Student")
        print("2. Display All Records")
        print("3. Binary Search")
        print("4. Sort Students (Insertion Sort)")
        print("5. Update Record")
        print("6. Delete Record")
        print("7. Exit")

        choice = int(input("Enter your choice: "))

        if choice == 1:
            add_student()
        elif choice == 2:
            display_all()
        elif choice == 3:
            binary_search()
        elif choice == 4:
            insertion_sort()
        elif choice == 5:
            update_record()
        elif choice == 6:
            delete_record()
        elif choice == 7:
            print("Exiting...")
            break
        else:
            print("Invalid Choice!")


# --------------------- Main ----------------------
if __name__ == "__main__":
    menu()