# fix_mtl.py
import re
import os

mtl_path = r"C:\Users\cummi\source\repos\Dead-Reckoning-Homebound\Dead-Reckoning-Homebound\assets\models\x-wing\x-wing-flyingv1.mtl"

with open(mtl_path, "r") as f:
    content = f.read()

def fix_path(match):
    directive = match.group(1)
    full_path = match.group(2)
    # normalize slashes and grab ONLY the filename
    filename = os.path.basename(full_path.replace("\\", "/"))
    return f"{directive} {filename}"

fixed = re.sub(r'(map_Kd|map_Ks|map_Ns|map_Bump|map_d|refl|bump)\s+(\S+)', fix_path, content)

with open(mtl_path, "w") as f:
    f.write(fixed)

print("Done!")