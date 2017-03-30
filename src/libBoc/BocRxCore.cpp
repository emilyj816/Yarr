#include "BocRxCore.h"
#include <cstring>

BocRxCore::BocRxCore()
{
	m_enableMask = 0;

	for(int ch = 0; ch < 32; ch++)
	{
		m_formState[ch] = 0;
	}
}

BocRxCore::~BocRxCore()
{
	// disable all channels
	for(int ch = 0; ch < 32; ch++)
	{
		// channel is disabled
		if(ch >= 16)
		{
			m_com->writeSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x0);
		}
		else
		{
			m_com->writeSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x0);
		}
	}
}

void BocRxCore::setRxEnable(uint32_t val)
{
	// save mask locally
	m_enableMask = val;

	// loop through channels and enable them
	for(int ch = 0; ch < 32; ch++)
	{
		if(m_enableMask & (1 << ch))
		{
			// channel is enabled
			if(ch >= 16)
			{
				m_com->writeSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x5);
			}
			else
			{
				m_com->writeSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x5);
			}			
		}
		else
		{
			// channel is disabled
			if(ch >= 16)
			{
				m_com->writeSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x0);
			}
			else
			{
				m_com->writeSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_CTRL, 0x0);
			}
		}
	}
}

void BocRxCore::maskRxEnable(uint32_t val, uint32_t mask)
{
	uint32_t tmp = m_enableMask;
    tmp &= ~mask;
    val |= tmp;
    setRxEnable(val);
}

RawData* BocRxCore::readData()
{
	std::vector<uint32_t> formatted_data;

	// loop through all enabled channels and fetch the FIFO data
	for(int ch = 0; ch < 32; ch++)
	{
		// skip disabled channels
		if((m_enableMask & (1 << ch)) == 0) continue;

		// skip empty channels
		if(ch >= 16)
		{
			if(m_com->readSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20)
				continue;

			while((m_com->readSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20) == 0)
			{
				uint16_t data;
				uint8_t tmp[2];
				m_com->readInc(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_DATA_HIGH, tmp, 2);
				data  = (uint16_t) tmp[0] << 8;
				data |= (uint16_t) tmp[1] << 0;
				m_rxData[ch].push(data);
			}
		}
		else
		{
			if(m_com->readSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20)
				continue;

			while((m_com->readSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20) == 0)
			{
				uint16_t data;
				uint8_t tmp[2];
				m_com->readInc(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_DATA_HIGH, tmp, 2);
				data  = (uint16_t) tmp[0] << 8;
				data |= (uint16_t) tmp[1] << 0;
				m_rxData[ch].push(data);
			}
		}
	}

	// let's format the data
	for(int ch = 0; ch < 32; ch++)
	{
		// build records
		while(m_rxData[ch].size() >= 4)
		{
			uint16_t data = m_rxData[ch].front();
			m_rxData[ch].pop();

			// synchronize on start of frame
			if((data & 0x1FF) == 0x1FC)
			{
				m_formState[ch] = 0;
			}
			else if((data & 0x100) == 0)
			{
				switch(m_formState[ch])
				{
					case 0:
						m_formRecord[ch] = (uint32_t)(data & 0xFF) << 16;
						m_formState[ch]++;
						break;

					case 1:
						m_formRecord[ch] |= (uint32_t)(data & 0xFF) <<  8;
						m_formState[ch]++;
						break;

					case 2:
						m_formRecord[ch] |= (uint32_t)(data & 0xFF) << 0;
						m_formState[ch] = 0;
						formatted_data.push_back((ch << 26) | (0x1 << 24) | m_formRecord[ch]);
						break;

				}
			}
		}
	}

	// return the data to caller
	if(formatted_data.size() == 0)
	{
		return NULL;
	}
	else
	{
		uint32_t *buf = new uint32_t[formatted_data.size()];
		std::copy(formatted_data.begin(), formatted_data.end(), buf);
		return new RawData(0x0, buf, formatted_data.size());
	}
}

uint32_t BocRxCore::getDataRate()
{
	return 0;
}

uint32_t BocRxCore::getCurCount()
{
	return 0;
}

bool BocRxCore::isBridgeEmpty()
{
	for(int ch = 0; ch < 32; ch++)
	{
		// skip disabled channels
		if((m_enableMask & (1 << ch)) == 0) continue;

		// channel is disabled
		if(ch >= 16)
		{
			if((m_com->readSingle(BMFS_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20) == 0) {
				return false;
			}
		}
		else
		{
			if((m_com->readSingle(BMFN_OFFSET + BMF_RX_OFFSET + 32 * (ch%16) + BMF_RX_STATUS) & 0x20) == 0) {
				return false;
			}
		}
	}

	// if we reach this point no channel had data
	return true;
}
