from __future__ import annotations

import json
from pydantic import(
    BaseModel,
    ValidationError,
    Field,
)


class Wheel(BaseModel):
    length_mm: float = Field(default=15.0)
    diameter_mm: float = Field(default=5.0)


class Keyboard(BaseModel):
    pitch_mm: float = Field(default=1.0)


class Music(BaseModel):
    sample_rate: int = Field(default=44100)


class Config(BaseModel):
    wheel: Wheel = Field(default_factory=Wheel)
    keyboard: Keyboard = Field(default_factory=Keyboard)
    music: Music = Field(default_factory=Music)

    @classmethod
    def load(cls, file_path: str) -> Config:
        try:
            with open(file_path, 'r') as file:
                data = json.load(file)
            config = Config(**data)
            return config
        except FileNotFoundError:
            print("Configuration file not found.")
            return Config()
        except ValidationError as e:
            print("Configuration validation error:", e)
            return Config()


if __name__ == "__main__":
    config = Config.load("config/config.json")
    print(config)
