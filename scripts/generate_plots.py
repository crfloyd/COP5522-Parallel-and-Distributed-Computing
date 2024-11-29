import os
from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Get project root directory (parent of scripts directory)
PROJECT_ROOT = Path(__file__).parent.parent
RESULTS_PATH = PROJECT_ROOT / 'data' / 'results' / 'performance_results.csv'
OUTPUT_PATH = PROJECT_ROOT / 'data' / 'results' / 'plots'

# Create plots directory if it doesn't exist
os.makedirs(OUTPUT_PATH, exist_ok=True)

# Read the CSV file
df = pd.read_csv(RESULTS_PATH)

# Set matplotlib styles
plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

# 1. Speedup Analysis
def plot_speedup():
    # Calculate speedup using serial times as baseline
    serial_times = df[df['Implementation'] == 'Serial'].set_index(['Vertices', 'Density'])['Time(ms)']
    parallel_df = df[df['Implementation'] != 'Serial'].copy()
    
    parallel_df['Speedup'] = parallel_df.apply(
        lambda row: serial_times.loc[row['Vertices'], row['Density']] / row['Time(ms)'],
        axis=1
    )

    # Create plot for N=1000
    plt.figure(figsize=(10, 6))
    for impl in ['OpenMP', 'MPI']:
        data = parallel_df[
            (parallel_df['Implementation'] == impl) & 
            (parallel_df['Vertices'] == 1000)
        ]
        if impl == 'OpenMP':
            plt.plot(data['Threads/Processes'], data['Speedup'], 
                    marker='o', label=impl)
        else:
            plt.scatter(data['Threads/Processes'], data['Speedup'], 
                       marker='s', label=impl, s=100)

    plt.plot([1, 8], [1, 8], 'k--', label='Linear Speedup')
    plt.xlabel('Number of Threads/Processes')
    plt.ylabel('Speedup')
    plt.title('Speedup Analysis (N=1000)')
    plt.legend()
    plt.grid(True)
    plt.savefig(OUTPUT_PATH / 'speedup_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()

# 2. Strong Scaling Efficiency
def plot_efficiency():
    # Calculate efficiency for OpenMP
    openmp_df = df[df['Implementation'] == 'OpenMP'].copy()
    baseline_times = openmp_df[openmp_df['Threads/Processes'] == 1].set_index(['Vertices', 'Density'])['Time(ms)']
    
    openmp_df['Efficiency'] = openmp_df.apply(
        lambda row: (baseline_times.loc[row['Vertices'], row['Density']] / 
                    (row['Time(ms)'] * row['Threads/Processes'])) * 100,
        axis=1
    )

    plt.figure(figsize=(10, 6))
    for size in [200, 500, 1000]:
        data = openmp_df[openmp_df['Vertices'] == size]
        plt.plot(data['Threads/Processes'], data['Efficiency'], 
                marker='o', label=f'N={size}')

    plt.xlabel('Number of Threads')
    plt.ylabel('Parallel Efficiency (%)')
    plt.title('Strong Scaling Efficiency (OpenMP)')
    plt.legend()
    plt.grid(True)
    plt.savefig(OUTPUT_PATH / 'scaling_efficiency.png', dpi=300, bbox_inches='tight')
    plt.close()

# 3. Execution Time vs Problem Size
def plot_execution_time():
    plt.figure(figsize=(10, 6))
    
    for impl in ['Serial', 'OpenMP', 'MPI']:
        data = df[df['Implementation'] == impl]
        if impl == 'OpenMP':
            data = data[data['Threads/Processes'] == 8]  # Use 8 threads for OpenMP
        mean_times = data.groupby('Vertices')['Time(ms)'].mean()
        plt.plot(mean_times.index, mean_times.values, 
                marker='o', label=f'{impl}')

    plt.yscale('log')
    plt.xscale('log')
    plt.xlabel('Number of Vertices (N)')
    plt.ylabel('Execution Time (ms)')
    plt.title('Execution Time vs Problem Size')
    plt.legend()
    plt.grid(True)
    plt.savefig(OUTPUT_PATH / 'execution_time.png', dpi=300, bbox_inches='tight')
    plt.close()

# 4. Density Impact Analysis
def plot_density_impact():
    plt.figure(figsize=(10, 6))
    
    for density in [0.1, 0.3, 0.5]:
        data = df[
            (df['Implementation'] == 'OpenMP') & 
            (df['Vertices'] == 1000) & 
            (df['Density'] == density)
        ]
        plt.plot(data['Threads/Processes'], data['Time(ms)'], 
                marker='o', label=f'Density={density}')

    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (ms)')
    plt.title('Impact of Graph Density on OpenMP Performance (N=1000)')
    plt.legend()
    plt.grid(True)
    plt.savefig(OUTPUT_PATH / 'density_impact.png', dpi=300, bbox_inches='tight')
    plt.close()

# Generate all plots
if __name__ == '__main__':
    plot_speedup()
    plot_efficiency()
    plot_execution_time()
    plot_density_impact()