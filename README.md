# pre-requisites: make, nasm and gcc.

Debian/Ubuntu/etc: sudo apt install make nasm gcc -y

Fedora/CentOS/etc: sudo dnf install make nasm gcc -y

# Clone the repository.

git clone https://github.com/raviagiri/check-memcpy

cd check-memcpy

make

# Change OS CPU governor to 'performance'

echo "performance" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Run the benchmark.

sudo  ./check-memcpy 1 # libc memcpy

sudo  ./check-memcpy 2 # memcpy_avx256

sudo  ./check-memcpy 3 # memcpy_avx512

sudo  ./check-memcpy 4 # memcpy_movsb

# License
This is free and unencumbered software released into the public domain
