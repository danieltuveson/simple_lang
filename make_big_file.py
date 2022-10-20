
input_file = 'examples/declare.vb'
big_file = 'examples/big_file.vb'

with open(input_file, encoding='utf-8') as file:
    content = file.read()

with open(big_file, mode='w+', encoding='utf-8') as file:
    for i in range(0, 9999):
        file.write(content)

