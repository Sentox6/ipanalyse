#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

void read_input(char *buffer, int length);
void print_address(uint8_t address[4], char *prompt);

int main(int argc, char *argv[])
{
    char input[19] = "";

    while (1)
    {
        printf("Please enter an IP address:\n");
        read_input(input, 19);

        // Convert the IP address into an array of bytes
        uint8_t address[4];
        char *pos = input;
        for (int i = 3; i >= 0; i--, pos++)
        {
            // The array must be ordered 'backwards' to preserve bit order
            address[i] = strtol(pos, &pos, 10);
        }
        int subnet = strtol(pos, NULL, 10);

        // Create an int pointer to our byte array to perform bit math
        uint32_t *addr_bits = (uint32_t *)address;

        // Calculate the network address using the subnet mask
        uint32_t sub_mask = UINT_MAX << (32 - subnet);
        *addr_bits &= sub_mask;
        print_address(address, "Network Address:   ");

        // Calculate the broadcast address
        *addr_bits |= UINT_MAX >> subnet;
        print_address(address, "Broadcast Address: ");

        // Print the subnet mask in decimal value
        *addr_bits = sub_mask;
        print_address(address, "Subnet Mask:       ");
        
        // Print the maximum number of host addresses
        int hosts = (1 << (32 - subnet)) - 2;
        printf("Network Hosts:     %d\n", hosts);
    }
}

void read_input(char *buffer, int length)
{
    if (length <= 0)
        return;

    int size = 0, c;

    // Read all input until CR (mac), LF (linux), or input is exhausted
    while ((c = getchar()) != '\r' && c != '\n' && c != EOF)
    {
        if (size + 1 == length)
            continue;
        buffer[size++] = c;

    }
    buffer[size] = '\0';
}

void print_address(uint8_t address[4], char *prompt)
{
    printf("%s", prompt);
    for (int i = 3; i >= 0; i--)
    {
        printf("%d", address[i]);
        printf(i == 0 ? "\n" : ".");
    }
}