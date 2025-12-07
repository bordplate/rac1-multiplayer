//
// Created by bordplate on 11/24/2025.
//

#include "DataClient.h"
#include <netex/errno.h>

#include "../../rc1.h"
#include "rc1/Game.h"

bool DataClient::stop = false;
DataClient* DataClient::data_client_ = nullptr;

ssize_t send_all(int sockfd, const void *buf, size_t len) {
    const u8 *p = (const u8 *)buf;
    size_t total = 0;

    while (total < len) {
        ssize_t n = send(sockfd, p + total, len - total, 0);
        if (n < 0) {
            if (sys_net_errno == EINTR) {
                continue;
            }

            return -1;
        }
        if (n == 0) {
            break;
        }
        total += (size_t)n;
    }

    return (ssize_t)total;
}

DataClient::DataClient(sockaddr_in* sockaddr, u32 key) : sockfd_(), key_(), should_transmit_(), file_to_transmit_(), file_size_(), transmitted_() {
    sockaddr_ = sockaddr;
    running_ = false;

    stop = false;

    key_ = key;

    data_client_ = this;
    is_receiving_ = false;
}

void DataClient::_connect() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd_, (sockaddr*)sockaddr_, sizeof(sockaddr_in)) != -1) {
        running_ = true;
        send(sockfd_, &key_, 4, 0);

        Logger::debug("Connected data stream over TCP");

        return;
    }

    Logger::error("Failed to connect data stream over TCP");
}

void DataClient::run() {
    Logger::debug("Starting data stream runloop");
    u8 buffer[512];

    while (running_ && !stop) {
        if (recv(sockfd_, &buffer, sizeof(MPPacketHeader), MSG_DONTWAIT) > 0) {
            MPPacketHeader* header = (MPPacketHeader*)buffer;

            switch (header->type) {
                case MP_PACKET_FILE_UPLOAD: {
                    is_receiving_ = true;
                    MPPacketFileUpload file_upload;
                    if (recv(sockfd_, &file_upload, sizeof(MPPacketFileUpload), 0) < 0) {
                        Logger::error("Failed to receive file upload packet correctly.");
                        return;
                    }

                    void* target_buffer = nullptr;
                    size_t target_size = file_upload.len;

                    Logger::info("Downloading file type %d; %d kb", file_upload.file_type, file_upload.len / 1024);

                    switch (file_upload.file_type) {
                        case MPFileTypeSavefile: {
                            target_buffer = (void*)0x1000000;
                            break;
                        }
                        default: {
                            Logger::error("Server tried to send invalid file type %d", file_upload.file_type);
                            return;
                        }
                    }

                    if (target_buffer == nullptr) {
                        Logger::error("Target buffer for file download of type %d is nullptr.", file_upload.file_type);
                        return;
                    }

                    size_t downloaded = 0;

                    while (downloaded < target_size) {
                        size_t chunk_size = 4096;
                        if (chunk_size > target_size - downloaded) {
                            chunk_size = target_size - downloaded;
                        }

                        int d = recv(sockfd_, (u8*)target_buffer + downloaded, chunk_size, 0);

                        if (d < 0) {
                            Logger::error("Error downloading file type %d; Downloaded %d bytes", file_upload.file_type, downloaded);
                            return;
                        } else if (d == 0) {
                            Logger::warning("Recvd 0 bytes for file type %d; Downloaded %d bytes", file_upload.file_type, downloaded);
                        }

                        downloaded += (size_t)d;
                    }

                    Logger::info("Downloaded file type %d; %d kb", file_upload.file_type, downloaded / 1024);

                    is_receiving_ = false;
                    Game::shared().have_save_file = true;

                    break;
                }
                default: {
                    Logger::warning("Server sent unknown packet type %d over TCP", header->type);
                    running_ = false;
                    stop = true;
                    return;
                }
            }
        }

        if (should_transmit_) {
            size_t packet_size = sizeof(MPPacketHeader) + sizeof(MPPacketFileUpload);
            u8 send_buffer[packet_size];
            MPPacketHeader header = {
                MP_PACKET_FILE_UPLOAD, 0, sizeof(MPPacketFileUpload), 0, 0, 0
            };
            MPPacketFileUpload upload = {
                transmit_file_type_,
                file_size_,
            };

            memcpy(send_buffer, &header, sizeof(header));
            memcpy(send_buffer + sizeof(header), &upload, sizeof(upload));

            send(sockfd_, &send_buffer, packet_size, 0);

            size_t remaining = file_size_;
            do {
                size_t chunk_size = 4096;
                if (chunk_size > remaining) {
                    chunk_size = remaining;
                }

                ssize_t sent = send_all(sockfd_, (u8*)file_to_transmit_ + transmitted_, chunk_size);

                transmitted_ += sent;
                remaining -= sent;
            } while (remaining > 0);

            should_transmit_ = false;
        }

        sys_timer_usleep(1000);
    }
}

void DataClient::transmit(MPFileType file_type, void *data, size_t len) {
    if (!data_client_->should_transmit_) {
        data_client_->should_transmit_ = true;

        data_client_->file_to_transmit_ = data;
        data_client_->file_size_ = len;
        data_client_->transmit_file_type_ = file_type;
        data_client_->transmitted_ = 0;
    }
}