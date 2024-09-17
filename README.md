# Synchronous and Asynchronous TCP Server Designs

This repository explores various concurrent server designs to evaluate their performance under different conditions. It develops and optimizes a concurrent server program using multiple processes, threads, and I/O multiplexing techniques. A custom NAT Network setup is implemented on VirtualBox to facilitate intercommunication between VMs. This project aims to provide a detailed analysis of performance metrics such as average throughput, latency, CPU, and memory utilization across different server models.

## Performance Analysis

The server is tested under varying loads to observe performance dynamics. Metrics like sending and receiving throughput are calculated using `tcpDump` on the server machine, ensuring reliable measurement of server performance.

### Specifications

- **CPU**: 3 cores
- **RAM**: 4 GB

### Tested Models

1. **Fork**
   - Client Counts: 500, 1000, 3000
2. **Multi-thread (pthreads)**
   - Client Counts: 500, 1000, 3000
3. **I/O Multiplexing**
   - Techniques: select, poll, epoll
   - Client Counts: 500, 1000 (for select); 500, 1000, 3000 (for poll and epoll)

### Metrics

- **Throughput**: Split into sending throughput and receiving throughput.
- **CPU Utilization**: Increases with the number of clients.
- **Memory Utilization**: Varies significantly between models, with fork and multi-threading having higher usage.

## Tech Stack

- **I/O Multiplexing Techniques**: select, poll, epoll
- **Socket Programming**
- **NAT Networking**
- **VirtualBox**
- **Wireshark** for network traffic analysis

## Usage

Detail on how to set up and run the server is beneficial here. Include any scripts or commands necessary to start the server and analyze the results.

## Results

Provide a summary of findings from the performance tests, possibly including tables or graphs if available.

## Contributing

Encourage other developers to contribute to the project by providing guidelines on how they can contribute.

## License

Specify the type of license under which this project is released.

