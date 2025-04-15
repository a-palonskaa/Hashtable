import pycallgrind

# Открытие и парсинг файла
callgrind_file = 'callgrind.out.389606.out'
with open(callgrind_file, 'r') as file:
    data = file.read()

# Преобразование в структуру данных
parsed_data = pycallgrind.parse(data)

# Пример вывода данных
for fn, details in parsed_data.items():
    print(f"Function: {fn}")
    print(f"  Calls: {details['calls']}")
    print(f"  Time: {details['time']}")
    print(f"  Callers: {details['callers']}")
    print(f"  Callees: {details['callees']}")
