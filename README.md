# Library Management System - Technical Documentation

## Code Structure and Architecture

This document provides technical details about the Library Management System implementation, focusing on design patterns, class relationships, and implementation specifics.

## Class Hierarchy and Relationships

### Inheritance Hierarchies

1. **Library Material Hierarchy**:
   - Library (Base) → Books → EBook/ResearchJournal

2. **Member Hierarchy**:
   - Member (Abstract Base) → Student/Faculty

3. **Exception Hierarchy**:
   - exception (STL) → LibraryException → Various specialized exceptions

### Aggregation/Composition Relationships

- `LibrarySystem` aggregates collections of Books, Members, Librarians, and Transactions
- `Member` contains issued book IDs
- `Books` contains member IDs for reservations

## Key Algorithms and Processes

### Book Issuing Algorithm
A multi-step process that finds the book and member, validates availability and permissions, then updates records.

### Fine Calculation Logic
Calculates late days and applies a daily rate (Rs. 10 per day).

### Reservation System Implementation
Uses vectors to track member reservations for books and a queue to maintain reservation order.

## Data Structure Usage

### STL Containers
- Lists for book, member, and transaction collections
- Stack for recent transactions (LIFO)
- Queue for reservations (FIFO)
- Vectors for tracking reservations and issued books

### Memory Management
Uses dynamic memory allocation without explicit deletion, which could lead to memory leaks.

## Polymorphism and Virtual Functions

- Virtual display functions in Books hierarchy
- Virtual inputDetails and display methods in Member hierarchy
- Pure virtual getMaxBooks function implemented in derived classes
- Dynamic binding using base class pointers

## Exception Handling Implementation

Custom exceptions derived from std::exception handle various error scenarios with consistent try-catch implementation.

## Critical Functions and Methods

- `LibrarySystem::run()`: Main entry point for user interaction
- `LibrarySystem::issueBook()` and `returnBook()`: Core circulation functionality
- `Transaction::calculateFine()`: Fee calculation based on dates
- `Books::update_availability()`: Updates book status

## Implementation Details

- Simple integer-based day counter for date management
- Basic password authentication for librarians
- Sample data generation for testing

## System Limitations

1. No persistent storage
2. Simplified date handling
3. Basic authentication
4. Poor memory management
5. Limited input validation

## Code Optimization Opportunities

1. Replace raw pointers with smart pointers
2. Implement move semantics
3. Add database integration
4. Improve input validation
5. Optimize search algorithms
6. Improve memory management

## Compilation and Execution

Simple compilation with g++ and standard execution.

## Testing Strategy

- Unit tests for basic operations
- Integration tests for complete workflows
- Edge case testing for error handling

## Conclusion

The system demonstrates object-oriented design principles with clear separation of concerns. While it has limitations, it provides a solid foundation that could be extended for production use.
