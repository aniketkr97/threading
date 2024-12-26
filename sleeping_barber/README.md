# **Sleeping Barber Problem**

## **Problem Statement**

The **Sleeping Barber Problem** is a classic synchronization problem in computer science, designed to illustrate inter-process communication and the use of condition variables.

In this problem:

1. A barber runs a barber shop with a fixed number of chairs for waiting customers.
2. If no customers are present, the barber goes to sleep.
3. Customers arrive at the shop:
   - If there is an available chair, they wait their turn for a haircut.
   - If all chairs are occupied, the customer leaves without getting a haircut.
4. When the barber finishes a haircut, they call the next customer from the waiting area. If no customers are waiting, the barber goes back to sleep.
5. The program runs until all customers have been processed or the shop is closed.

The objective is to simulate this scenario with proper synchronization, ensuring that:
- The barber and customers do not access shared resources (like the waiting room queue) simultaneously.
- The barber only cuts hair when a customer is available.
- No customer is served out of turn.

---

## **Key Features**

- Supports configurable numbers of waiting chairs and customers.
- Simulates real-world conditions with:
  - Customers arriving at random intervals.
  - Barber sleeping when there are no customers.
- Uses mutex locks and condition variables for synchronization.
- Tracks statistics:
  - Number of customers served.
  - Number of customers turned away due to a full waiting room.
- Ensures proper termination of the barber thread when the shop closes.

---

## **How It Works**

### **Barber Logic**
- The barber sleeps if no customers are waiting.
- When a customer arrives, the barber wakes up, cuts their hair, and then checks for more waiting customers.
- If no more customers are waiting and the shop is closing, the barber exits gracefully.

### **Customer Logic**
- A customer enters the shop and checks for an available waiting chair.
- If a chair is available, the customer takes a seat and waits for their turn.
- If no chairs are available, the customer leaves.
- When the barber is ready, the customer gets their haircut.

### **Termination**
- The shop closes after processing all customers.
- The barber thread is notified to terminate if no customers are left.

---

## **Implementation Details**

- **Language**: C++  
- **Synchronization**: Uses `std::mutex` and `std::condition_variable` to manage access to shared resources.
- **Queue**: Represents the waiting room, ensuring customers are served in a first-come, first-served manner.

---

## **Usage**

### **Compile and Run**

1. Clone this repository or copy the source files into your project.
2. Compile the program using a C++ compiler:
   ```bash
   g++ -std=c++17 -pthread sleeping_barber.cpp -o sleeping_barber

