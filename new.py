from z3 import *

# Define the order of the ring
n = 8  # Change this to your desired order

# Given the addition table (must be an abelian group)
# Example for Z/4Z (should be customized for your specific group)
# addition_table = [
#     [0, 1, 2, 3],
#     [1, 2, 3, 0],
#     [2, 3, 0, 1],
#     [3, 0, 1, 2]
# ]

# Klein Four-Group (V4) Addition Table
# addition_table = [
#     [0, 1, 2, 3],  # 0 + {0, a, b, c}
#     [1, 0, 3, 2],  # a + {0, a, b, c}
#     [2, 3, 0, 1],  # b + {0, a, b, c}
#     [3, 2, 1, 0]   # c + {0, a, b, c}
# ]

# Z8 Addition Table

addition_table = [
    [0, 1, 2, 3, 4, 5, 6, 7],
    [1, 2, 3, 4, 5, 6, 7, 0],
    [2, 3, 4, 5, 6, 7, 0, 1],
    [3, 4, 5, 6, 7, 0, 1, 2],
    [4, 5, 6, 7, 0, 1, 2, 3],
    [5, 6, 7, 0, 1, 2, 3, 4],
    [6, 7, 0, 1, 2, 3, 4, 5],
    [7, 0, 1, 2, 3, 4, 5, 6]
]

# Define Z3 functions for multiplication and addition lookup
multiplication = Function('mul', IntSort(), IntSort(), IntSort())

# Define a Z3 function to represent addition (instead of directly indexing into a list)
addition = Function('add', IntSort(), IntSort(), IntSort())

# Create constraints to encode the given addition table into the Z3 function
solver = Solver()
for i in range(n):
    for j in range(n):
        solver.add(addition(i, j) == addition_table[i][j])

# Multiplication is closed under the given set (values must be within 0 to n-1)
for i in range(n):
    for j in range(n):
        solver.add(And(multiplication(i, j) >= 0, multiplication(i, j) < n))

# Associativity: (a * b) * c == a * (b * c)
for i in range(n):
    for j in range(n):
        for k in range(n):
            solver.add(multiplication(multiplication(i, j), k) == multiplication(i, multiplication(j, k)))

# Distributivity: (a + b) * c == a * c + b * c and c * (a + b) == c * a + c * b
for i in range(n):
    for j in range(n):
        for k in range(n):
            solver.add(multiplication(addition(i, j), k) == addition(multiplication(i, k), multiplication(j, k)))
            solver.add(multiplication(k, addition(i, j)) == addition(multiplication(k, i), multiplication(k, j)))

# Identity element: There exists e such that e * a = a * e = a for all a
# e = Int('e')  # Identity element
# solver.add(And(e >= 0, e < n))
# for i in range(n):
#     solver.add(multiplication(i, e) == i)
#     solver.add(multiplication(e, i) == i)

# Find all solutions
solutions = []
while solver.check() == sat:
    model = solver.model()
    
    # Extract solution
    result_table = [[model.evaluate(multiplication(i, j)).as_long() for j in range(n)] for i in range(n)]
    
    # Print solution
    print("Multiplication Table:")
    for row in result_table:
        print(row)
    print("-" * 30)
    
    # Store solution
    solutions.append(result_table)
    
    # Block the current solution to find a new one
    solver.add(Or([multiplication(i, j) != result_table[i][j] for i in range(n) for j in range(n)]))

# Print number of solutions found
print(f"Total solutions found: {len(solutions)}")
