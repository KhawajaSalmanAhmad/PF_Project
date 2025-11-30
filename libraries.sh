echo "Installing required libraries for Switchback Rails..."

# Update package list
sudo apt-get update

# Install GCC and G++ compilers
sudo apt-get install -y build-essential g++ gcc

# Install SFML libraries
sudo apt-get install -y libsfml-dev

# Install additional dependencies
sudo apt-get install -y cmake make

# Install OpenGL and X11 libraries (required by SFML)
sudo apt-get install -y libgl1-mesa-dev libx11-dev

echo "All libraries installed successfully!"
echo "You can now run: make clean && make"
