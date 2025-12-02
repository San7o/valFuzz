## plotting

Create a virtual python environment:

```bash
python3 -m venv .venv
```

Enter the python edevelopement environment

```bash
source .venv/bin/activate
```

Install the requirements:

```bash
pip install -r requirements.txt
```

Create a kernel for jupyter:

```bash
python -m ipykernel install --user --name=venv
```

Run jupyter notebook:

```bash
jupyter lab
```

You then need to change the kernel to the previously created one.
