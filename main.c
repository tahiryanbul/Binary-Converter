#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void byte_to_binary(unsigned char byte, char *output) {
    for (int i = 7; i >= 0; i--) {
        output[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    output[8] = '\0';
}

void analyze_byte_detailed(unsigned char byte, int index, FILE *out) {
    char binary[9];
    byte_to_binary(byte, binary);
    fprintf(out, "Byte %04d: 0x%02X | ASCII: %c | 2's Comp: %4d | Binary: %s\n",
           index, byte,
           isprint(byte) ? byte : '.',
           (char)byte,
           binary);
    printf("Byte %04d: 0x%02X | ASCII: %c | 2's Comp: %4d | Binary: %s\n",
           index, byte,
           isprint(byte) ? byte : '.',
           (char)byte,
           binary);
}

void analyze_byte_ascii(unsigned char byte, FILE *out) {
    char c = isprint(byte) ? byte : '.';
    fprintf(out, "%c", c);
    printf("%c", c);
}

int main() {
    char filename[100];
    int mode;

    printf("Enter binary file name: ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Choose mode:\n");
    printf("1 - Detailed binary analysis\n");
    printf("2 - ASCII only output\n");
    printf("3 - File info only\n");
    printf("Enter your choice: ");
    if (scanf("%d", &mode) != 1 || mode < 1 || mode > 3) {
        printf("Invalid mode.\n");
        return 1;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("File could not be opened");
        return 1;
    }

    // Açılacak çıktı dosyasının adı (örneğin "output.txt")
    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        perror("Output file could not be created");
        fclose(fp);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    fprintf(out, "--- File Analysis Started ---\n");
    fprintf(out, "File name: %s\n", filename);
    fprintf(out, "File size: %ld bytes\n\n", filesize);

    printf("\n--- File Analysis Started ---\n");
    printf("File size: %ld bytes\n\n", filesize);

    unsigned char byte;
    int index = 0;

    if (mode == 3) {
        fprintf(out, "--- File info only ---\n");
        fprintf(out, "File name: %s\n", filename);
        fprintf(out, "File size: %ld bytes\n", filesize);

        printf("--- File info only ---\n");
        printf("File name: %s\n", filename);
        printf("File size: %ld bytes\n", filesize);

        fclose(fp);
        fclose(out);
        return 0;
    }

    while (fread(&byte, sizeof(unsigned char), 1, fp) == 1) {
        if (mode == 1) {
            analyze_byte_detailed(byte, index, out);
            if ((index + 1) % 16 == 0) {
                fprintf(out, "\n");
                printf("\n");
            }
        } else if (mode == 2) {
            analyze_byte_ascii(byte, out);
        }
        index++;
    }

    if (mode == 2) {
        fprintf(out, "\n");
        printf("\n");
    }

    fprintf(out, "\n--- File Analysis Finished. Total %d bytes read. ---\n", index);
    printf("\n--- File Analysis Finished. Total %d bytes read. ---\n", index);

    fclose(fp);
    fclose(out);

    printf("Analysis results saved to output.txt\n");

    return 0;
}
