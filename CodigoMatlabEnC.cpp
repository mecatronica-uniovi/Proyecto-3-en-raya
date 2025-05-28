#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define SERVER_IP "192.168.12.111"
#define SERVER_PORT 5000
#define IMAGE_FILENAME "imagen.jpg"

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    // Crear socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    // Dirección del servidor
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Conectar al servidor
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error al conectar al ESP32." << std::endl;
        return 1;
    }
    std::cout << "Conectado al ESP32." << std::endl;

    // Enviar solicitud de captura
    char cmd = 'C';
    send(sock, &cmd, 1, 0);

    // Leer tamaño de la imagen (4 bytes)
    uint8_t size_buf[4];
    recv(sock, (char*)size_buf, 4, MSG_WAITALL);
    uint32_t img_size;
    memcpy(&img_size, size_buf, 4);

    // Leer datos de imagen
    std::vector<uint8_t> img_data(img_size);
    recv(sock, (char*)img_data.data(), img_size, MSG_WAITALL);

    // Guardar imagen
    std::ofstream img_file(IMAGE_FILENAME, std::ios::binary);
    img_file.write(reinterpret_cast<char*>(img_data.data()), img_data.size());
    img_file.close();
    std::cout << "Imagen guardada como " << IMAGE_FILENAME << std::endl;

    // Leer con OpenCV
    cv::Mat image = cv::imread(IMAGE_FILENAME);
    if (image.empty()) {
        std::cerr << "Error al cargar la imagen." << std::endl;
        return 1;
    }

    // Convertir a espacio Lab
    cv::Mat lab_image;
    cv::cvtColor(image, lab_image, cv::COLOR_BGR2Lab);
    std::vector<cv::Mat> lab_channels;
    cv::split(lab_image, lab_channels);

    cv::Mat a_channel = lab_channels[1];
    cv::Mat b_channel = lab_channels[2];

    // Máscaras
    cv::Mat red_mask = a_channel > 150;
    cv::Mat blue_mask = b_channel < 110;

    // Mostrar imagen
    cv::imshow("Imagen", image);
    cv::waitKey(0);

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return 0;
}
