
DMIPS = 2000
FILE_PATH = 'output.txt'
CPU_IDX = 2

def calculate_cpu_average(file_path):
    cpu_values = []

    cpu_load_sum = 0
    idx = 0

    with open(file_path, 'r') as file:
        for line in file:
            parts = line.split()

            if len(parts) > 2:
                cpu_load_sum += float(parts[CPU_IDX])
                idx += 1
    
    avg_cpu_load = cpu_load_sum/idx
    print(f"Average CPU Usage : {avg_cpu_load:.2f}")
    print(f"DMIPS : {(avg_cpu_load * DMIPS * 0.01):.2f}")


# 평균 계산 및 출력
average_cpu = calculate_cpu_average(FILE_PATH)