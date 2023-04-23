void usb_rx_ok(void)
{
	switch (ll_usb_rx_buffer[1])
	{
		case USB_PID_SETUP:
			if ((ll_usb_rx_buffer[2] & 0x7F) == usb.device_address)
			{
				usb.stage = USB_STAGE_SETUP;
				usb.active_EP_num = 0;//(usb_rx_buffer[2] & 0x80)?1:0;
			} 
			else 
				usb.stage = USB_STAGE_NONE;
			break;
		
		case USB_PID_OUT:
			if ((ll_usb_rx_buffer[2] & 0x7F) == usb.device_address)
			{
				usb.stage = USB_STAGE_OUT;
				usb.active_EP_num = (uint8_t)((ll_usb_rx_buffer[2] & 0x80)?1:0);
			} 
			else 
				usb.stage = USB_STAGE_NONE;
			break;
		
		case USB_PID_IN:
			if ((ll_usb_rx_buffer[2] & 0x7F) == usb.device_address)
			{
				usb.active_EP_num = (uint8_t)((ll_usb_rx_buffer[2] & 0x80)?1:0);
				if (usb.EP[usb.active_EP_num].tx_state == USB_EP_DATA_READY) 
				{
					usb_send_packet(usb.active_EP_num);
					usb.EP[usb.active_EP_num].tx_state = USB_EP_NO_DATA;
					if (usb.setup_address) 
					{
						usb.device_address = usb.setup_address;
						usb.setup_address = 0;
					}
				}	
				else 
				{
					usb_send_nack();
				}
#if (USB_EP_WATCHDOG_ENABLE == 1)
				if (usb.WDG_EP_timeout > 0) 
					usb.WDG_EP_timeout = 0;
#endif
			}
			usb.stage = USB_STAGE_NONE;
			break;
		
		case USB_PID_DATA0: // Data received
		case USB_PID_DATA1: // Data received
			if (usb.stage != USB_STAGE_NONE) {
				if (usb.EP[usb.active_EP_num].rx_state == USB_EP_NO_DATA) // if EP ready
				{
					usb.EP[usb.active_EP_num].rx_state = USB_EP_DATA_READY;
					usb_send_ack();
					if (usb.active_EP_num == 0) 
						usb.EP0_data_stage = usb.stage; // USB_STAGE_SETUP or USB_STAGE_OUT
					usb.EP[usb.active_EP_num].rx_length = (uint8_t)(14 - ll_usb_rx_count);
					if (usb.EP[usb.active_EP_num].rx_length > 3) {
						usb.EP[usb.active_EP_num].rx_length -= 3; // 1..9
						usb_copy_rx_packet(usb.active_EP_num);
					} 
					else 
					usb.EP[usb.active_EP_num].rx_length = 0;
				}
				usb.stage = USB_STAGE_NONE;
			}
			break;

		//case USB_PID_ACK:
		//case USB_PID_NACK:		
		default:
			break;
	}
	if (usb.trimming_stage == HSI_TRIMMER_ENABLE)
		usb.trimming_stage = HSI_TRIMMER_STARTED;
}
