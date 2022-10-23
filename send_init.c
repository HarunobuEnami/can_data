int s;
	int nbytes;
    int mtu;
    int enable_canfd = 1;
  static	unsigned char key[AES_BLOCK_SIZE];
	unsigned char MAC[AES_BLOCK_SIZE];
	int p_size=4;
	int nsec;
	static uint32_t seq=0;
	int p_size=4; //データ長
	int nsec;
	static uint32_t seq=0;

	uint8_t plain []= {0x01,0x02,0x03,0x04};

	
	
	printf("-------------------------------------------------\n");
	printf("key            ");
	for(int i=0;i<AES_BLOCK_SIZE;i++){
		printf("%x",key[i]);
		if((i%4) == 3)
			printf(" ");
	}
	printf("\n");
    strcpy(ifr->ifr_name, ifname);
    ifr->ifr_ifindex = if_nametoindex(ifr->ifr_name);
	if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}

    if (ioctl(s, SIOCGIFMTU, ifr) < 0) {
			perror("SIOCGIFMTU");
			return 1;
		}
		mtu = ifr->ifr_mtu;

		if (mtu != CANFD_MTU) {
			printf("CAN interface is not CAN FD capable - sorry.\n");
			return 1;
		}

    if (setsockopt(s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
			       &enable_canfd, sizeof(enable_canfd))){
			printf("error when enabling CAN FD support\n");
			return 1;
		}
    
    frame->len = can_fd_dlc2len(can_fd_len2dlc(frame->len));
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

	strcpy(ifr->ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, ifr);

	addr->can_family = AF_CAN;
	addr->can_ifindex = ifr->ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr->ifr_ifindex);
	if(bind(s,(struct sockaddr *)addr, sizeof(*addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}
