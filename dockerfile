# On retire la contrainte amd64 pour utiliser l'architecture native de ton Mac
FROM ubuntu:22.04

# Désactive les invites interactives pendant l'installation
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    valgrind \
    gdb \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# INDISPENSABLE : Garde le conteneur allumé en tâche de fond
CMD ["tail", "-f", "/dev/null"]