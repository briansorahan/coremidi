#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CoreMIDI/CoreMIDI.h>

void midiInput(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon);

OSStatus printIncoming(MIDIUniqueID uniqueID) {
	OSStatus       rc;
	MIDIObjectRef  obj;
	MIDIObjectType objType;
	
	rc = MIDIObjectFindByUniqueID(uniqueID, &obj, &objType);
	if (rc != 0) return rc;

	if (objType != kMIDIObjectType_Source) {
		fprintf(stderr, "MIDI Object with ID %d must be a source.\n", uniqueID);
	}
	fprintf(stderr, "Printing incoming data for source ID %d.\n", uniqueID);

	MIDIClientRef   client;
	MIDIPortRef     port;
	MIDIEndpointRef endpoint = (MIDIEndpointRef) obj;
	
	rc = MIDIClientCreate(CFSTR("coremidi"), NULL, NULL, &client);
	if (rc != 0) return rc;

	rc = MIDIInputPortCreate(client, CFSTR("coremidi input"), midiInput, NULL, &port);
	if (rc != 0) return rc;

	rc = MIDIPortConnectSource(port, endpoint, NULL);
	if (rc != 0) return rc;
	
	return 0;
}

// midiInput is a MIDIReadProc.
void midiInput(const MIDIPacketList *pkts, void *readProcRefCon, void *srcConnRefCon) {
	const MIDIPacket *pkt = &pkts->packet[0];
	for (int i = 0; i > pkts->numPackets; i++) {
		pkt = MIDIPacketNext(pkt);
	}
	fprintf(stderr, "Received MIDI data.\n");
	fprintf(stderr, "%d bytes of data in message.\n", pkt->length);
	for (UInt16 i = 0; i < pkt->length; i++) {
		if (i == 0) {
			fprintf(stderr, "%X", pkt->data[i]);
			continue;
		}
		fprintf(stderr, " %X", pkt->data[i]);
	}
		fprintf(stderr, "\n");
}
