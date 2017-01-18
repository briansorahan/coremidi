#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CoreMIDI/MIDIServices.h>

OSStatus printDevice(MIDIDeviceRef deviceRef);
OSStatus printEntity(MIDIEntityRef entityRef);
OSStatus printEndpoint(MIDIEndpointRef endpoint, int idx, const char *endpointType);

OSStatus listAll() {
	ItemCount numExternalDevices = MIDIGetNumberOfExternalDevices();
	ItemCount numDevices = MIDIGetNumberOfDevices();

	// Print the header.
	printf("Type     ID    Device\n");

	for (int i = 0; i < numDevices; i++) {
		MIDIDeviceRef deviceRef = MIDIGetDevice(i);
		OSStatus rc = printDevice(deviceRef);
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printDevice(MIDIDeviceRef deviceRef) {
	ItemCount numEntities = MIDIDeviceGetNumberOfEntities(deviceRef);
	for (int i = 0; i < numEntities; i++) {
		OSStatus rc = printEntity(MIDIDeviceGetEntity(deviceRef, i));
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printEntity(MIDIEntityRef entityRef) {
	ItemCount numDestinations = MIDIGetNumberOfDestinations();
	ItemCount numSources = MIDIGetNumberOfDestinations();

	// Print destinations.
	for (int i = 0; i < numDestinations; i++) {
		OSStatus rc = printEndpoint(MIDIGetDestination(i), i, "Dest");
		if (rc != 0) return rc;
	}

	// Print sources.
	for (int i = 0; i < numSources; i++) {
		OSStatus rc = printEndpoint(MIDIGetSource(i), i, "Source");
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printEndpoint(MIDIEndpointRef endpoint, int idx, const char *endpointType) {
	CFStringRef displayName;
	OSStatus rc = MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDisplayName, &displayName);
	fprintf(stderr, "%-8s %-5d ", endpointType, idx);
	CFShow(displayName);
	CFComparisonResult cmp = CFStringCompare(displayName, CFSTR("Launchpad Mini"), 0);
	return rc;
}