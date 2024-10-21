# Update submodules (ssh key should not have a password, otherwise this can be tedious)
cd ../
git submodule update --init --remote --recursive
git submodule update

# Setup software bus network (ignore all warnings)
cd apps/sbn
./install ../..
cd ../../scripts
