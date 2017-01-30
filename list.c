#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/MIDIServices.h>

OSStatus printDevice(MIDIDeviceRef deviceRef, int i);
OSStatus printEntity(MIDIEntityRef entityRef);
OSStatus printEndpoint(MIDIEndpointRef endpoint, const char *endpointType);
char *CFStringToUTF8(CFStringRef aString);

OSStatus listAll() {
	ItemCount numDevices = MIDIGetNumberOfDevices();

	fprintf(stderr, "%lu devices\n", numDevices);

	for (int i = 0; i < numDevices; i++) {
		MIDIDeviceRef deviceRef = MIDIGetDevice(i);
		OSStatus rc = printDevice(deviceRef, i);
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printDevice(MIDIDeviceRef deviceRef, int deviceIndex) {
	CFStringRef  deviceName;
	ItemCount    numEntities = MIDIDeviceGetNumberOfEntities(deviceRef);
	OSStatus     rc;

	rc = MIDIObjectGetStringProperty(deviceRef, kMIDIPropertyName, &deviceName);
	if (rc != 0) return rc;

	fprintf(stderr, "device %s, %lu %s\n", CFStringToUTF8(deviceName), numEntities, (numEntities == 1 ? "entity" : "entities"));

	for (int i = 0; i < numEntities; i++) {
		rc = printEntity(MIDIDeviceGetEntity(deviceRef, i));
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printEntity(MIDIEntityRef entityRef) {
	CFStringRef entityName;
	ItemCount   numDestinations = MIDIGetNumberOfDestinations();
	ItemCount   numSources = MIDIGetNumberOfDestinations();
	OSStatus    rc;

	rc = MIDIObjectGetStringProperty(entityRef, kMIDIPropertyName, &entityName);
	if (rc != 0) return rc;

	fprintf(stderr, "entity %s, %lu %s, %lu %s\n", CFStringToUTF8(entityName), numDestinations, (numDestinations == 1 ? "destination" : "destinations"), numSources, (numSources == 1 ? "source" : "sources"));

	// Print destinations.
	for (int i = 0; i < numDestinations; i++) {
		rc = printEndpoint(MIDIGetDestination(i), "Dest");
		if (rc != 0) return rc;
	}
	// Print sources.
	for (int i = 0; i < numSources; i++) {
		rc = printEndpoint(MIDIGetSource(i), "Source");
		if (rc != 0) return rc;
	}
	return 0;
}

OSStatus printEndpoint(MIDIEndpointRef endpoint, const char *endpointType) {
	CFStringRef displayName;
	MIDIUniqueID uniqueID;
	
	OSStatus rc = MIDIObjectGetStringProperty(endpoint, kMIDIPropertyDisplayName, &displayName);
	if (rc != 0) return rc;
	
	rc = MIDIObjectGetIntegerProperty(endpoint, kMIDIPropertyUniqueID, &uniqueID);
	if (rc != 0) return rc;
						   
	fprintf(stderr, "\t%-8s %-12d\n", endpointType, uniqueID);
	
	return rc;
}

char *CFStringToUTF8(CFStringRef aString) {
	if (aString == NULL) {
		return NULL;
	}

	CFIndex length = CFStringGetLength(aString);
	CFIndex maxSize =
		CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
	char *buffer = (char *)malloc(maxSize);
	if (CFStringGetCString(aString, buffer, maxSize,
			       kCFStringEncodingUTF8)) {
		return buffer;
	}
	free(buffer); // If we failed
	return NULL;
}
