#include "BackgroundRenderer.h"

BackgroundRenderer::BackgroundRenderer(const Shader& shader)
    : m_shader(shader)
{
    // Due triangoli in coordinate normalizzate NDC [-1,+1].
    // Non occorrono Model Matrix ne' matrice di proiezione.
    const float vertices[] = {
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BackgroundRenderer::~BackgroundRenderer()
{
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void BackgroundRenderer::draw(float timeSeconds) const
{
    // Usa il programma shader dello sfondo, diverso da quello del gioco.
    m_shader.use();
    // glGetUniformLocation richiede un program ID, non GL_CURRENT_PROGRAM:
    // leggiamo l'ID attivo per mantenere la classe Shader invariata.
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    const GLint timeLocation = glGetUniformLocation(static_cast<GLuint>(currentProgram), "uTime");
    if (timeLocation >= 0) glUniform1f(timeLocation, timeSeconds);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
