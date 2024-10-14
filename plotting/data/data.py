import pandas as pd


class data:
    def parse_data(path):
        data = pd.read_csv(path)
        return data

    def get_test_data():
        data = pd.read_csv('test_data.txt')
        return data
