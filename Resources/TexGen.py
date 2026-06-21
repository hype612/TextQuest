from PIL import Image, ImageOps
from pathlib import Path

ascii_texture = " .:coPO?@#"
# ascii_texture = " :P?#"
# ascii_texture = " :P#"


def get_closest_pow2(n: int):
    if n <= 0:
        raise ValueError("Input must be a positive Integer")

    l_pow2 = 1
    while l_pow2 * 2 <= n:
        l_pow2 *= 2

    h_pow2 = l_pow2 * 2
    if n - l_pow2 <= h_pow2 - n:
        return l_pow2
    else:
        return h_pow2


def create_char(l: int):
    idx = min(l * len(ascii_texture) // 256, len(ascii_texture) - 1)
    return ascii_texture[idx]


parent_path = Path("./TextureSource")
tex_files = list(parent_path.iterdir())

# height and with of a char
char_size = 4
step = 256 // len(ascii_texture)
shadow_levels = 8

for texSource in tex_files:
    img_in = Image.open(texSource).convert("RGBA")
    bg = Image.new("RGB", img_in.size, (0, 0, 0))
    bg.paste(img_in, mask=img_in.split()[-1])
    img_in = bg

    new_size = tuple(int(dim / char_size) for dim in img_in.size)
    new_size = tuple(get_closest_pow2(i) for i in new_size)
    tex = img_in.resize(new_size)
    tex = tex.convert("L")
    tex = ImageOps.autocontrast(tex, cutoff=2)
    texData = tex.load()   # no quantize() at all

    for lv in range(shadow_levels):
        tex_name = "./Textures/" + \
            str(texSource.stem) + "lv" + str(lv) + ".txt"
        with open(tex_name, "w", encoding="utf-8") as file:
            for i in range(new_size[1]):
                for j in range(new_size[0]):
                    out = max(0, texData[j, i] - lv * step)
                    file.write(create_char(out))
                file.write("\n")
        img_in.close()
