#!/bin/bash

PHONEBOOK="phonebook.txt"

# Create file if not exists
if [ ! -f "$PHONEBOOK" ]; then
    touch "$PHONEBOOK"
fi

# Function to add a new entry
add_entry() {
    echo "Enter First Name:"
    read fname
    echo "Enter Last Name:"
    read lname
    echo "Enter Phone Number:"
    read phone
     
    echo -e "${fname}\t${lname}\t${phone}" >> "$PHONEBOOK"
    echo "Entry added successfully!"
}

# Function to search by name or phone number
search_entry() {
    echo "Enter name or phone number to search:"
    read search
    grep -i "$search" "$PHONEBOOK" || echo "No match found."
}

# Function to sort by last name
sort_phonebook() {
    sort -k2,2 "$PHONEBOOK" -o "$PHONEBOOK"
    echo "Phonebook sorted by last name."
}

# Function to delete an entry
delete_entry() {
    echo "Enter name or phone number of the entry to delete:"
    read search
    grep -vi "$search" "$PHONEBOOK" > temp.txt && mv temp.txt "$PHONEBOOK"
    echo "If the entry existed, it has been deleted."
}

# Menu-driven interface
while true; do
    echo
    echo "========= PHONEBOOK MENU ========="
    echo "1. Add new entry"
    echo "2. Search entry"
    echo "3. Sort phonebook by last name"
    echo "4. Delete entry"
    echo "5. Display all entries"
    echo "6. Exit"
    echo "=================================="
    echo "Enter your choice: "
    read choice

    case $choice in
        1) add_entry ;;
        2) search_entry ;;
        3) sort_phonebook ;;
        4) delete_entry ;;
        5) cat "$PHONEBOOK" ;;
        6) echo "Exiting..."; break ;;
        *) echo "Invalid choice. Try again." ;;
    esac
done
