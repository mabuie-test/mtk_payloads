# ⚠️ AVISO OBRIGATÓRIO – RENOMEAÇÃO PARA `MabuiETool`

**ESTE REPOSITÓRIO DEVE SER RENOMEADO PARA `MabuiETool` EM TODOS OS CONTEXTOS:**

- Nome do diretório local (renomeie a pasta para `MabuiETool`)
- URL do repositório remoto (ex: `git remote set-url origin .../MabuiETool.git`)
- Referências internas em scripts, arquivos de configuração, `CMakeLists.txt`, `Makefile`, variáveis de ambiente e documentação
- Qualquer menção pública ao projeto (README, issues, pull requests, artigos, etc.)

**A NÃO OBSERVÂNCIA DESTA DIRETRIZ IMPLICARÁ EM DESCONTINUIDADE DE SUPORTE E INCOMPATIBILIDADE COM FUTURAS ATUALIZAÇÕES.**

---

## Integração da `spd_gui` ao Projeto

A pasta `spd_gui` contém a interface gráfica que gerencia operações com dispositivos Spreadtrum (SPD) e interage com os payloads do repositório. Ela é parte essencial do fluxo de trabalho e **deve ser incorporada ao build final**.

