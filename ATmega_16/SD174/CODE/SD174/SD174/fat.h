void writeFile (char *fileName)
{
	unsigned char j, data, error, fileCreatedFlag = 0, start = 0, appendFile = 0, sector;
	unsigned int i, firstClusterHigh;//, firstClusterLow;
	unsigned int firstClusterLow;
	struct dir_Structure *dir;
	unsigned long cluster, nextCluster, prevCluster, firstSector, clusterCount, extraMemory;

	j = readFile (VERIFY, fileName);

	if(j == 1)
	{
		...some code...
	}
	else if(j == 2)
	return; //invalid file name
	else
	{
		...some code...
	}



	while(1)
	{
		if(start)
		{
			start = 0;
			startBlock = getFirstSector (cluster) + sector;
			SD_readSingleBlock (startBlock);
			i = fileSize % bytesPerSector;
			j = sector;
		}
		else
		{
			startBlock = getFirstSector (cluster);
			i=0;
			j=0;
		}
		

		TX_NEWLINE;
		transmitString_F(PSTR(" Enter text (end with ~):"));
		
		do
		{
			data = receiveByte();
			if(data == 0x08)	//'Back Space' key pressed
			{
				if(i != 0)
				{
					transmitByte(data);
					transmitByte(' ');
					transmitByte(data);
					i--;
					fileSize--;
				}
				continue;
			}
			transmitByte(data);
			buffer[i++] = data;
			fileSize++;
			if(data == '\r')  //'Carriege Return (CR)' character
			{
				transmitByte ('\n');
				buffer[i++] = '\n'; //appending 'Line Feed (LF)' character
				fileSize++;
			}
			
			if(i == 512)
			{
				i=0;
				error = SD_writeSingleBlock (startBlock);
				j++;
				if(j == sectorPerCluster) {j = 0; break;}
				startBlock++;
			}
		}while (data != '~');

		if(data == '~')
		{
			fileSize--;	//to remove the last entered '~' character
			i--;
			for(;i<512;i++)  //fill the rest of the buffer with 0x00
			buffer[i]= 0x00;
			error = SD_writeSingleBlock (startBlock);

			break;
		}
		
		prevCluster = cluster;

		cluster = searchNextFreeCluster(prevCluster); //look for a free cluster starting from the current cluster

		if(cluster == 0)
		{
			TX_NEWLINE;
			transmitString_F(PSTR(" No free cluster!"));
			return;
		}

		getSetNextCluster(prevCluster, SET, cluster);
		getSetNextCluster(cluster, SET, EOF);   //last cluster of the file, marked EOF
	}

	getSetFreeCluster (NEXT_FREE, SET, cluster); //update FSinfo next free cluster entry

	if(appendFile)  //executes this loop if file is to be appended
	{
		SD_readSingleBlock (appendFileSector);
		dir = (struct dir_Structure *) &buffer[appendFileLocation];
		extraMemory = fileSize - dir->fileSize;
		dir->fileSize = fileSize;
		SD_writeSingleBlock (appendFileSector);
		freeMemoryUpdate (REMOVE, extraMemory); //updating free memory count in FSinfo sector;

		
		TX_NEWLINE;
		transmitString_F(PSTR(" File appended!"));
		TX_NEWLINE;
		return;
	}

	//executes following portion when new file is created

	prevCluster = rootCluster; //root cluster

	while(1)
	{
		...Some Code..
		//adds time, date, etc. to created file
	}