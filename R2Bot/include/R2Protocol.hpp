#ifndef _R2_PROTOCOL
#define _R2_PROTOCOL

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace R2Protocol {
	struct Packet {
		std::string source;
		std::string destination;
		std::string id;
		std::vector<uint8_t> data;
		std::string checksum;
	};

	/**
	* Convert a two byte checksum to hex digits (big endian)
	*/
	inline std::string checksumToHex(uint16_t checksum) {
		char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		char digits[] = {
			hex[(checksum >> 12) & 0xf],
			hex[(checksum >> 8) & 0xf],
			hex[(checksum >> 4) & 0xf],
			hex[(checksum >> 0) & 0xf]
		};
		return std::string(digits);
	}

	/**
	* Compute the IPv4 checksum of data
	*/
	inline uint16_t computeChecksum(std::vector<uint8_t>& data, uint32_t start, uint32_t end) {
		uint32_t index;
		uint32_t sum = 0;
		for (index = start; index < end - 1; index += 2) {
			sum += (data[index] << 8) | data[index + 1];
			if (sum > 0xffff) {
				sum -= 0xffff;
			}
		}
		if (index < end) { // Odd byte
			sum += data[index] << 8;
			if (sum > 0xffff) {
				sum -= 0xffff;
			}
		}
		return (uint16_t)(sum & 0xffff);
	}

	inline uint32_t readString(std::vector<uint8_t>& buf, uint32_t index, uint32_t len, std::string& value) {
		std::vector<char> str(len);
		for (uint32_t i = 0; i < len; i++) {
			str[i] = (char)buf[i + index];
		}
		value = std::string(str.begin(), str.end());
		return len;
	}

	inline uint32_t readByte(std::vector<uint8_t>& buf, uint32_t index, uint8_t& value) {
		value = buf[index];
		return 1;
	}

	inline uint32_t readBytes(std::vector<uint8_t>& buf, uint32_t index, uint32_t len, std::vector<uint8_t>& value) {
		value.resize(len);
		for (uint32_t i = 0; i < len; i++) {
			value[i] = buf[i + index];
		}
		return len;
	}

	inline uint32_t readShort(std::vector<uint8_t>& buf, uint32_t index, uint16_t& value) {
		value = (buf[index] << 8) | buf[index + 1];
		return 2;
	}

	inline uint32_t readInt(std::vector<uint8_t>& buf, uint32_t index, uint32_t& value) {
		value = buf[index] | (buf[index + 1] << 8) | (buf[index + 2] << 16) | (buf[index + 3] << 24);
		return 4;
	}

	/**
	* Decode the data, returning the index where the data reading finishes, or negative if data failed to be read
	*/
	inline int32_t decode(std::vector<uint8_t>& input, Packet& params) {
		uint32_t start = 0;
		while (start < input.size() - 3 && !(input[start] == 'G' && input[start + 1] == '0' && input[start + 2] == '0')) {
			start++;
		}
		if (start >= input.size() - 3) {
			return -1;
		}

		uint32_t index = start + 3;
		char end = 0;
		while (!end && index < input.size()) {
			char key = input[index++];
			if (key == 'S') {
				uint8_t len;
				index += readByte(input, index, len);
				index += readString(input, index, len, params.source);
			}
			else if (key == 'D') {
				uint8_t len;
				index += readByte(input, index, len);
				index += readString(input, index, len, params.destination);
			}
			else if (key == 'T') {
				uint8_t len;
				index += readByte(input, index, len);
				index += readString(input, index, len, params.id);
			}
			else if (key == 'P') {
				uint32_t len;
				index += readInt(input, index, len);
				index += readBytes(input, index, len, params.data);
			}
			else if (key == 'K') {
				uint16_t checksum;
				uint16_t computedChecksum = computeChecksum(input, start, index - 1);
				uint8_t len;
				index += readByte(input, index, len);
				index += readShort(input, index, checksum);
				if (computedChecksum != checksum) {
					printf("Checksum does not match %d != %d\n", computedChecksum, checksum);
					return -1;
				}
				params.checksum = checksumToHex(checksum);
			}
			else if (key == 'G') {
				if (index + 2 < input.size() && input[index + 1] == '0' && input[index + 2] == '1') {
					index += 2;
					end = 1;
				}
			}
		}
		return index;
	}

	inline uint32_t writeString(std::vector<uint8_t>& buf, uint32_t index, std::string str) {
		for (int i = 0; i < str.length(); i++) {
			buf[i + index] = (uint8_t)str[i];
		}
		return str.length();
	}

	inline uint32_t writeByte(std::vector<uint8_t>& buf, uint32_t index, uint8_t value) {
		buf[index] = value;
		return 1;
	}

	inline uint32_t writeBytes(std::vector<uint8_t>& buf, uint32_t index, std::vector<uint8_t>& value) {
		for (int i = 0; i < value.size(); i++) {
			buf[i + index] = value[i];
		}
		return value.size();
	}

	inline uint32_t writeInt(std::vector<uint8_t>& buf, uint32_t index, uint32_t value) {
		buf[index] = (value >> 0) & 0xff;
		buf[index + 1] = (value >> 8) & 0xff;
		buf[index + 2] = (value >> 16) & 0xff;
		buf[index + 3] = (value >> 24) & 0xff;
		return 4;
	}

	/**
	* Encode the data, returning the size of the encoded data, or negative if data failed to be encoded
	*/
	inline int32_t encode(Packet params, std::vector<uint8_t>& output) {
		uint32_t len = 3 + // G00 start
			2 + // S{length1} source
			params.source.length() + // source
			2 + // D{length1} destination
			params.destination.length() + // destination
			2 + // T{length1} transaction id
			params.id.length() + // transaction id
			5 + // P{length4}
			params.data.size() + // data
			4 + // K{length}{data} checksum
			3; // G01 end
		output.resize(len);
		uint32_t index = 0;
		index += writeString(output, index, "G00");
		index += writeString(output, index, "S");
		index += writeByte(output, index, params.source.length());
		index += writeString(output, index, params.source);
		index += writeString(output, index, "D");
		index += writeByte(output, index, params.destination.length());
		index += writeString(output, index, params.destination);
		index += writeString(output, index, "T");
		index += writeByte(output, index, params.id.length());
		index += writeString(output, index, params.id);
		index += writeString(output, index, "P");
		index += writeInt(output, index, params.data.size());
		index += writeBytes(output, index, params.data);
		uint16_t checksum = computeChecksum(output, 0, index);
		index += writeString(output, index, "K");
		index += writeByte(output, index, 2);
		index += writeByte(output, index, (checksum >> 8) & 0xff);
		index += writeByte(output, index, (checksum >> 0) & 0xff);
		index += writeString(output, index, "G01");
		return len;
	}
}

#endif