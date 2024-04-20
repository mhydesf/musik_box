from argparse import ArgumentParser


def main():
    parser = ArgumentParser()
    parser.add_argument("-f", "--file", type=str, required=True,
                        help="file path to audio")
    args = parser.parse_args()

    file = args.file


if __name__ == "__main__":
    main()
