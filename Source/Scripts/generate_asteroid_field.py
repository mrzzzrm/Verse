import argparse
import math
import random
import json


def random_vector():
    return [
        random.uniform(-1.0, 1.0),
        random.uniform(-1.0, 1.0),
        random.uniform(-1.0, 1.0)
    ]


def vector_length(v):
    return math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])


def normalize_vector(v):
    l = vector_length(v)
    v[0] /= l
    v[1] /= l
    v[2] /= l
    return v


def random_unit_vector():
    while True:
        vec = random_vector()
        if vector_length(vec) <= 1.0:
            break

    return normalize_vector(vec)


def quat_from_axis_angle(axis, angle):
    qx = axis[0] * math.sin(angle / 2)
    qy = axis[1] * math.sin(angle / 2)
    qz = axis[2] * math.sin(angle / 2)
    qw = math.cos(angle / 2)

    return [qx, qy, qz, qw]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--position', nargs=3, type=float, default=[0, 0, 0])
    parser.add_argument('--size', nargs=3, type=float, default=[2000, 100, 2000])
    parser.add_argument('--num_asteroids', type=int, default=150)
    parser.add_argument('--asteroid_scale', type=float, nargs=2, default=[2.0, 10.0])
    parser.add_argument('--num_asteroid_prototypes', type=int, default=2)
    parser.add_argument('--angular_speed_range', type=float, nargs=2, default=[0.1, 0.3])
    args = parser.parse_args()

    entities = []

    for asteroid_idx in range(args.num_asteroids):
        prototype_idx = random.randint(0, args.num_asteroid_prototypes - 1)
        prototype_name = "Asteroid{:02d}".format(prototype_idx)

        position = [
            random.uniform(-args.size[0]/2.0, args.size[0]/2.0) + args.position[0],
            random.uniform(-args.size[1]/2.0, args.size[1]/2.0) + args.position[1],
            random.uniform(-args.size[2]/2.0, args.size[2]/2.0) + args.position[2]
        ]

        axis = random_unit_vector()
        angle = random.uniform(0.0, 2 * math.pi)

        scale = random.uniform(args.asteroid_scale[0], args.asteroid_scale[1])

        coriolis = random_unit_vector()
        coriolis_speed = random.uniform(args.angular_speed_range[0], args.angular_speed_range[1])
        coriolis = [coriolis_speed * e for e in coriolis]

        entities.append({
            "LID": "asteroid{}".format(asteroid_idx),
            "Name": "AsteroidFieldAsteroid{}".format(asteroid_idx),
            "Bases": [prototype_name],
            "Components": {
                "Transform3D": {
                    "Position": position,
                    "Orientation": quat_from_axis_angle(axis, angle),
                    "Scale": scale
                },
                "Coriolis": {
                    "AngularVelocity": coriolis
                }
            }
        })

    level = {
        "Entities": entities
    }

    print(json.dumps(level, indent=2))

