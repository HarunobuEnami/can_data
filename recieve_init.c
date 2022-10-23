int s;
    int i;
    int mtu;
    int enable_canfd = 1;
    const int dropmonitor_on = 1;
    int nbytes;
    uint8_t plain [4];
    can_err_mask_t err_mask = CAN_ERR_MASK;
    uint32_t seq_num;
    static int ini_flg = 1;
    //struct msghdr msg;
    struct iovec iov = {
		.iov_base = frame,
	};
  struct msghdr msg = {
		.msg_iov = &iov,
	};

    strcpy(ifr->ifr_name, ifname);  
     ifr->ifr_ifindex = if_nametoindex(ifr->ifr_name);  
     if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}
  
  addr->can_family = AF_CAN;
  addr->can_ifindex = ifr->ifr_ifindex;
  mtu = ifr->ifr_mtu;

    if(setsockopt(s,SOL_CAN_RAW,CAN_RAW_FD_FRAMES,&enable_canfd,sizeof(enable_canfd)))
      {
       printf("error when enabling CAN FD support\n");
       return 1;
      }
  frame->len = can_fd_dlc2len(can_fd_len2dlc(frame->len));
  if (setsockopt(s, SOL_SOCKET, SO_RXQ_OVFL,
		       &dropmonitor_on, sizeof(dropmonitor_on)) < 0) {
		perror("setsockopt() SO_RXQ_OVFL not supported by your Linux Kernel");
	}

  if (setsockopt(s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask))) {
		perror("setsockopt()");
		return 1;
	}
    if (bind(s, ((struct sockaddr *) addr), sizeof(*addr)) < 0) {
			perror("bind");
			return -1;
		}
    
    msg.msg_name = addr;
    msg.msg_flags = 0;