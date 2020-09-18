#include"include.h"
class ScanPort
{
public:
int tcp_connet(char *ip, int i) {
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);
	addr.sin_port = htons(i);
	if ( connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1 ) {
		return 0;
	} else  {
		close(sfd);
	}
	return 1;
}
void scan_port( void ) {
	char ip[32] = {};	
	int start_port;
	int end_port;
	
	printf("ip:");
	scanf("%s", ip);
	printf("start:");
	scanf("%d", &start_port);
	printf("end:");
	scanf("%d", &end_port);

	int i;
	for (i=start_port; i<=end_port; i++) {
		if ( tcp_connet(ip, i) == 1 ) {
			struct servent *ps = getservbyport(htons(i), "tcp");
			printf("%d, %s", i, (ps==NULL)?"unkown":ps->s_name);
		}
	}
}
};
