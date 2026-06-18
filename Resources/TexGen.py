from PIL import Image
from pathlib import Path

ascii_texture = " .:coPO?@#"
#ascii_texture = " :P?#"
#ascii_texture = " :P#"

def get_closest_pow2(n: int):
    if n <= 0:
        raise ValueError("Input must be a positive Integer")
    
    l_pow2 = 1
    while l_pow2 * 2 <=n:
        l_pow2 *= 2

    h_pow2 = l_pow2 * 2
    if n - l_pow2 <= h_pow2 - n:
        return l_pow2
    else:
        return h_pow2

def create_char(l: int):
     #lum = l / 255 * 10
     return ascii_texture[l]
    


parent_path = Path("./TextureSource")
tex_files = list(parent_path.iterdir())

# height and with of a char
char_size = 4 


for texSource in tex_files:
    img_in = Image.open(texSource)

    new_size = tuple(int(dim / char_size) for dim in img_in.size)
    new_size = tuple(get_closest_pow2(i) for i in new_size)
    tex = img_in.resize(new_size)
    tex = tex.convert("L")
    tex = tex.quantize(len(ascii_texture))
    texData = tex.load()

    tex_name = "./Textures/" + str(texSource.stem) + ".txt"
    levels = 8
    max_idx = len(ascii_texture) - 1

    for level in range(levels):   # 0 = original, higher = darker
        tex_name = f"./Textures/{texSource.stem}_dark{level}.txt"
    
        with open(tex_name, "w", encoding="utf-8") as file:
            for i in range(new_size[1]):
                for j in range(new_size[0]):
                    idx = texData[j, i] + level     # darker → higher index
                    idx = min(idx, max_idx)         # clamp
                    file.write(ascii_texture[idx])
                file.write("\n")
        img_in.close()
