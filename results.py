from typing import Callable
from dataclasses import dataclass
import re
import json
import plotly.express as px
import pandas as pd

BLUE = "\033[94m"
RED = "\033[94m"


def colored(color: str) -> Callable[[str], str]:
    RESET = "\033[0m"
    return lambda s: f"{color}{s}{RESET}"


def input_or_default(prompt: str, default: str) -> str:
    ret = input(prompt + f" (default: {default}): ")
    if ret == "":
        return default
    return prompt


@dataclass
class Benchmark:
    type_name: str
    bm_name: str
    time: float

    @staticmethod
    def from_json(j: dict) -> "Benchmark":
        full_name: str = j["name"]
        match = re.search(r"BM<(.+)>::(.+)", full_name)
        if match is None:
            raise RuntimeError("invalid json")
        type_name = match[1]
        bm_name = match[2]
        time = float(j["cpu_time"])
        return Benchmark(type_name, bm_name, time)


def group_by_bm_name(bms: list[Benchmark]) -> list[list[Benchmark]]:
    d: dict[str, list[Benchmark]] = {}
    for bm in bms:
        if bm.bm_name not in d:
            d[bm.bm_name] = []
        d[bm.bm_name].append(bm)
    return list(d.values())


def main():
    filename = input_or_default("benchmark output json", "./benchmark_results/out.json")
    baseline = input_or_default("baseline impl", "cpp_std_unordered_map")
    print(colored(BLUE)(f"filename = {filename}"))
    print(colored(BLUE)(f"baseline = {baseline}"))

    with open(filename) as f:
        j = json.loads(f.read())
        bms = list(map(Benchmark.from_json, j["benchmarks"]))

    baseline_bms = [bm for bm in bms if bm.type_name == baseline]

    if len(baseline_bms) == 0:
        print(colored(RED)("No such baseline '{baseline}', aborting"))

    baseline_cpu_time = {}
    for bm in baseline_bms:
        baseline_cpu_time[bm.bm_name] = bm.time

    for bm in bms:
        bm.time /= baseline_cpu_time[bm.bm_name]

    y_axis_name = f"Time vs {baseline}"

    df = pd.DataFrame(
        {
            "Benchmark": [bm.bm_name for bm in bms],
            "Type Name": [bm.type_name for bm in bms],
            y_axis_name: [bm.time for bm in bms],
        }
    )
    fig = px.bar(
        df,
        x="Benchmark",
        y=y_axis_name,
        color="Type Name",
        barmode="group",
    )
    fig.show()


if __name__ == "__main__":
    main()
