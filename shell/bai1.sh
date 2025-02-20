#!/bin/bash

name="Pham Duy Chien"
echo "Hello $name"
echo "Today is $(date)"
echo "Current working directory: $(pwd)"

address=""
read -p "Enter your address: " address
echo "Your address is: $address"

username=""
read -p "Enter your username: " username

password=""
read -sp "Enter your password: " password