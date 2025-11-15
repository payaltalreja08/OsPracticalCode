#!/bin/bash

ADDRESSBOOK="addressbook.txt"

# Create file if not exists
if [ ! -f "$ADDRESSBOOK" ]; then
    touch "$ADDRESSBOOK"
fi

# Function to display entries
display_entries() {
    echo "----- Address Book -----"
    if [ -s "$ADDRESSBOOK" ]; then
        cat "$ADDRESSBOOK" | column -t -s ":"
    else
        echo "Address book is empty."
    fi
}

# Function to search for an entry
search_entry() {
    echo "Enter name or phone number to search:"
    read search
    grep -i "$search" "$ADDRESSBOOK" | column -t -s ":" || echo "No match found."
}

# Function to add an entry
add_entry() {
    echo "Enter First Name:"
    read fname
    echo "Enter Last Name:"
    read lname
    echo "Enter Phone Number:"
    read phone
    echo "Enter Address:"
    read addr

    echo "${fname}:${lname}:${phone}:${addr}" >> "$ADDRESSBOOK"
    echo "Entry added successfully!"
}

# Function to remove an entry
remove_entry() {
    echo "Enter name or phone number to remove:"
    read search
    grep -vi "$search" "$ADDRESSBOOK" > temp.txt && mv temp.txt "$ADDRESSBOOK"
    echo "If the entry existed, it has been removed."
}

# Function to edit an entry
edit_entry() {
    echo "Enter name or phone number to edit:"
    read search
    line=$(grep -i "$search" "$ADDRESSBOOK")
    if [ -z "$line" ]; then
        echo "No match found."
        return
    fi

    echo "Existing Entry: $line"
    echo "Enter new details:"

    echo "First Name:"
    read fname
    echo "Last Name:"
    read lname
    echo "Phone Number:"
    read phone
    echo "Address:"
    read addr

    grep -vi "$search" "$ADDRESSBOOK" > temp.txt
    echo "${fname}:${lname}:${phone}:${addr}" >> temp.txt
    mv temp.txt "$ADDRESSBOOK"
    echo "Entry updated successfully!"
}

# Menu-driven program
while true; do
    echo
    echo "========= ADDRESS BOOK MENU ========="
    echo "1. Search address book"
    echo "2. Add entry"
    echo "3. Remove entry"
    echo "4. Edit entry"
    echo "5. Display all entries"
    echo "6. Exit"
    echo "====================================="
    echo "Enter your choice: "
    read choice

    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_entry ;;
        4) edit_entry ;;
        5) display_entries ;;
        6) echo "Exiting..."; break ;;
        *) echo "Invalid choice. Try again." ;;
    esac
done
