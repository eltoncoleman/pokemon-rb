/*
 * Compresses or decompresses the Town Map tilemap
 * from Pokemon Red, Blue, and Yellow.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
usage()
{
	fprintf(stderr, "Usage: redrle [-d] infile outfile\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	FILE *infile, *outfile;
	bool d = false; /* compress or decompress flag */

	if (argc < 3 || argc > 4)
		usage();

	if (strcmp(argv[1], "-d") == 0) {
		if (argc != 4)
			usage();
		d = true;
	}

	infile = fopen(argv[argc - 2], "rb");
	if (infile == NULL) {
		fprintf(stderr, "Error opening file '%s': ", argv[argc - 2]);
		perror(NULL);
		exit(1);
	}

	outfile = fopen(argv[argc - 1], "wb");
	if (outfile == NULL) {
		fprintf(stderr, "Error opening file '%s': ", argv[argc - 1]);
		perror(NULL);
		exit(1);
	}

	if (d) { /* decompress */
		for (;;) {
			int i, count;
			int byte = fgetc(infile);
			if (byte == 0)
				break;
			count = byte & 0xF;
			byte >>= 4;

			if (feof(infile)) {
				fprintf(stderr, "Decompress error: reached "
				    "end of file without finding terminating "
				    "null byte.\n");
				exit(1);
			}
			for (i = 0; i < count;
			    ++i)
				fputc(byte, outfile);
		}
	} else { /* compress */
		int byte, count = 0, lastbyte = 0;
		for (;;) {
			byte = fgetc(infile);

			if (feof(infile)) {
				while (count > 0xF) {
					count -= 0xF;
					fputc(lastbyte << 4 | 0xF, outfile);
				}
				if (count != 0) {
					fputc(lastbyte << 4 | count, outfile);
				}
				break;
			}

			if (byte > 0xF) {
				fprintf(stderr, "Compress error: read a byte "
				    "greater than 0xF.\n");
				exit(1);
			}

			if (byte == lastbyte)
				++count;
			else {
				while (count > 0xF) {
					count -= 0xF;
					fputc(lastbyte << 4 | 0xF, outfile);
				}
				if (count != 0) {
					fputc(lastbyte << 4 | count, outfile);
					count = 0;
				}

				lastbyte = byte;
				count = 1;
			}
		}

		fputc(0, outfile); /* Terminating 0x00 */
	}

	fclose(infile);
	fclose(outfile);

	return 0;
}
