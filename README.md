# Workshop Sistemas Operacionais

## Requerimentos

- Docker
- Suporte a `nested virtualization`
- Processador x86_64

VSCode é recomendado.

## Windows

Siga esse guia da Microsoft de como configurar seu VSCode com Docker no WSL2.

https://code.visualstudio.com/blogs/2020/03/02/docker-in-wsl2

## Testando

Para testar se está tudo certo, só executar os comandos a seguir.

```sh
docker build -t workshop .
docker run --device /dev/kvm --mount type=bind,source="$PWD",target=/workshop --rm -it workshop /bin/bash
```

E dentro do terminal, rode.

```sh
cd kvm
make run
```

Se a mensagem `KVM_EXIT_HLT` apareceu, então está tudo pronto para você começar a brincar.

## Tarefa

Crie um fork do repositório.

E implemente a operação de `open` em um arquivo. https://man7.org/linux/man-pages/man2/open.2.html

Similar como o `write` foi implementado.
