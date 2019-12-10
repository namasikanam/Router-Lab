#include <stdio.h>
#include <pcap.h>

int main() {
    char error_buffer[PCAP_ERRBUF_SIZE];
    // if (pcap_open_live("eth0", BUFSIZ, 1, 1, error_buffer)) { // Succeed
    if (pcap_open_live("veth-net0", BUFSIZ, 1, 1, error_buffer)) { // Fail
    // if (pcap_open_live("lo", BUFSIZ, 1, 1, error_buffer)) { // Fail
        printf("Succeed\n");
    }
    else {
        fprintf(stderr, "Fail\n");
    }
}