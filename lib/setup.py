from setuptools import setup, find_packages


DESCRIPTION = """
Library for Python music box application, which
generates a drum for a music box to play any input
audio file.
"""


with open("../requirements.txt") as f:
    requirements = [l.strip() for l in f.readlines() if not l.startswith("#")]


setup(
    name="musik_box",
    version="0.0.1",
    author="Mikhail Hyde",
    author_email="hyde.mikhail@gmail.com",
    description=DESCRIPTION,
    packages=find_packages(),
    install_requires=requirements
)
