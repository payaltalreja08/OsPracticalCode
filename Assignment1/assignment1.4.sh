#!/bin/bash


# ---------- Function to test if file exists ----------
test_file_exists() {
    read -p "Enter filename to test: " filename
    if [ -f "$filename" ]; then
        echo "File '$filename' exists."
    else
        echo "File '$filename' does not exist."
    fi
}

# ---------- Function to read a file ----------
read_file() {
    read -p "Enter filename to read: " filename
    if [ -f "$filename" ]; then
        echo "---- Contents of '$filename' ----"
        cat "$filename"
        echo "---------------------------------"
    else
        echo "File '$filename' not found!"
    fi
}

# ---------- Function to delete a file ----------
delete_file() {
    read -p "Enter filename to delete: " filename
    if [ -f "$filename" ]; then
        read -p "Are you sure you want to delete '$filename'? (y/n): " confirm
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            rm "$filename"
            echo "File '$filename' deleted."
        else
            echo "Deletion cancelled."
        fi
    else
        echo "File '$filename' not found!"
    fi
}

# ---------- Function to display list of files ----------
display_files() {
    echo "Files in current directory:"
    ls -1
}

# ---------- Menu Loop ----------
while true; do
    echo ""
    echo "=================================="
    echo "     FILE MANAGEMENT PROGRAM"
    echo "=================================="
    echo "1. Test if file exists"
    echo "2. Read a file"
    echo "3. Delete a file"
    echo "4. Display list of files"
    echo "5. Exit"
    echo "----------------------------------"
    read -p "Enter your choice [1-5]: " choice

    case $choice in
        1) test_file_exists ;;
        2) read_file ;;
        3) delete_file ;;
        4) display_files ;;
        5) echo "Exiting program..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
done
