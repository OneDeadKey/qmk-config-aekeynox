FROM qmkfm/qmk_cli:latest

ARG QMK_VERSION=0.32.3
ARG QMK_KEYBOARD=beekeeb/piantor

# Install QMK firmware at pinned version
RUN qmk setup -y -b $QMK_VERSION

# Configure QMK defaults
RUN qmk config user.keyboard=$QMK_KEYBOARD \
 && qmk config user.keymap=default \
 && qmk config user.qmk_home=/qmk_firmware

# Working directory for tests
WORKDIR /workdir

# Copy source
COPY . /workdir/

ENV KEYBOARD=$QMK_KEYBOARD
ENV QMK_HOME="/qmk_firmware"

HEALTHCHECK --interval=30s --timeout=10s --retries=3 \
    CMD qmk --version || exit 1

ENTRYPOINT ["bash"]
CMD ["qmk/tests/run_all.sh"]
