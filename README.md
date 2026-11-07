# AimTrainer Windows x64 - correção v2

Esta versão corrige `min`/`max` não encontrados incluindo `<algorithm>` e usando
`(std::min)` / `(std::max)` para evitar macros do Windows. Também remove macros
duplicadas e a variável não utilizada.

Envie o conteúdo desta pasta para a raiz do repositório GitHub (não apenas o ZIP).
Depois execute Actions > Build AimTrainer Windows x64 e baixe o artefato
`AimTrainer-Windows-x64-Release`.

Controles: clique no alvo para pontuar; R reinicia; + / - altera o tamanho; Esc fecha.
Aplicativo de treino independente, sem interação com CS2.
