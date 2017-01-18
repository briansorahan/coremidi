#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <CoreMIDI/MIDIServices.h>

OSStatus listAll();
void usage();

int main(int argc, char **argv) {
	// Process command line options
	if (argc <= 1) {
		usage(); exit(0);
	}
	
	OSStatus rc;
	char *device;
	
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			usage();
		} else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
			rc = listAll();
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
			if (argc <= i) usage(); exit(1);
			device = argv[i+1];
			i++;
		} else {
			printf("Unrecognized option: %s", argv[i]);
		}
	}
	if (rc != 0) {
		printf("Error code %d\n", rc);
	}
}

void usage() {
	fprintf(stderr, "Usage: coremidi [OPTIONS]\n");
	fprintf(stderr, "OPTIONS\n");
	fprintf(stderr, "-h|--help                  Print this help message.\n");
	fprintf(stderr, "-l|--list                  List all devices and entities.\n");
	fprintf(stderr, "-d|--device                Connect to a particular device.\n");
}
