from itertools import product

def generate_cpp(templates):
  result_string =  "// This file is generated by template_generator.py\n\n"
  result_string += "array all_expressions = to_array<size_t>({0b%s});\n\n" % ', 0b'.join(templates);
  result_string += "size_t func(int a, int b, int c, int d, int e, int f, op op1, op op2, op op3, op op4, op op5, int goal){\n\tsize_t res = 0;\n"
  for ind, expr in enumerate(templates):
    stack = []
    variables = list('abcdef')[::-1]
    op_number = 1
    for char in expr[::-1]:
      match(char):
        case '1': stack.append(variables.pop())
        case '0':
          b, a = stack.pop(), stack.pop()
          stack.append(f'op{op_number}({a}, {b})')
          op_number += 1
    result_string += f'\tres |= static_cast<size_t>(goal == {stack[-1]}) << {ind}; // {expr}\n'
  result_string += "\treturn res;\n}\n"
  return result_string


# In the expression template 1 -- is a number, 0 is an operation
# Expression is valid if
# 1. length is one of 1, 3, 5, 7, 9, 11
# 2. Number of '1's is more that number of '0's in any suffix
# 3. Number of '1's is exactly one more that number of '0's for the whole expression

# L -- length of the expression
# N -- number of numbers
# Z -- number of operators
# N + Z = L and  N - Z = 1, hence 2*N = L + 1

# Case with length 1 is skipped, because resulting template count
# was 65, one too many to fit into 64 bit int

def suffixes(ls):
  for i in range(len(ls)):
    yield ls[-i-1:]

def conditions(el: list[int]):
  for suff in suffixes(el):
    if sum(suff)*2 <= len(suff): return False
  if sum(el)*2 != len(el) + 1:  return False
  return True

def generate_templates():
  a = ["".join(str(s) for s in c) 
          for N in range(3,13,2) 
            for c in product([0, 1], repeat=N) 
              if conditions(c) ]
  return a

cpp = generate_cpp(generate_templates())

print(cpp)

# templates = generate_templates()
# counter = 0
# for template in templates[::-1]:
  # for sf in suffixes(template):
    # if sf in templates and sf != template:
      # counter += 1
      # print(f"{counter}) {sf} suffix of {template}")