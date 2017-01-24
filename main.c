#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <CoreMIDI/MIDIServices.h>

OSStatus listAll();
OSStatus printIncoming(MIDIUniqueID uniqueID);
void usage();

int main(int argc, char **argv) {
	// Process command line options
	if (argc <= 1) {
		usage(); exit(0);
	}
	
	OSStatus     rc;
	MIDIUniqueID uniqueID;
	int          block = 0;
	
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			usage();
		} else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
			rc = listAll();
		} else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--print") == 0) {
			if (uniqueID == 0) {
				fprintf(stderr, "--id must be provided first\n");
			}
			rc = printIncoming(uniqueID);
			block = 1;
		} else if (strcmp(argv[i], "--id") == 0) {
			if (argc < i) {
				fprintf(stderr, "--id option requires an argument\n");
				usage(); exit(1);
			}
			sscanf(argv[i+1], "%" SCNd32, &uniqueID);
			i++;
		} else {
			printf("Unrecognized option: %s", argv[i]);
		}
	}
	if (rc != 0) {
		printf("Error code %d\n", rc);
	}
	if (block > 0) {
		CFRunLoopRun();
	}
}

void usage() {
	fprintf(stderr, "Usage: coremidi [OPTIONS]\n");
	fprintf(stderr, "OPTIONS\n");
	fprintf(stderr, "-h, --help       Print this help message.\n");
	fprintf(stderr, "-l, --list       List all devices and entities.\n");
	fprintf(stderr, "--id             Unique ID of a device/entity/endpoint.\n");
	fprintf(stderr, "-p, --print      Print incoming data from a particular deivce. This requires the --id option first.\n");
}
