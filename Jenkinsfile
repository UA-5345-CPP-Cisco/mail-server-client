pipeline {
    agent any

    options {
        skipDefaultCheckout(true)
        disableConcurrentBuilds()
        timestamps()
        timeout(time: 45, unit: 'MINUTES')
    }

    triggers {
        pollSCM('H/5 * * * *')
    }

    environment {
        DEPLOY_HOST = '10.26.0.53'
        DEPLOY_DIR  = '/home/student/mail-server-client'
        SSH_PORT    = '22'
    }

    stages {
        stage('Checkout main') {
            steps {
                deleteDir()
                checkout scm

                sh '''#!/usr/bin/env bash
                    set -Eeuo pipefail

                    echo "Checked out commit:"
                    git log -1 --oneline
                '''
            }
        }

        stage('Validate repository') {
            steps {
                sh '''#!/usr/bin/env bash
                    set -Eeuo pipefail

                    test -f docker-compose.yml
                    test -f apps/mail-server/Dockerfile
                    test -f apps/smtp-server/Dockerfile

                    echo "Required deployment files are present."
                '''
            }
        }

        stage('Deploy to Docker machine') {
            steps {
                withCredentials([
                    sshUserPrivateKey(
                        credentialsId: 'docker-machine-ssh',
                        keyFileVariable: 'SSH_KEY',
                        usernameVariable: 'SSH_USER'
                    )
                ]) {
                    sh '''#!/usr/bin/env bash
                        set -Eeuo pipefail

                        TARGET="${SSH_USER}@${DEPLOY_HOST}"
                        KNOWN_HOSTS="${HOME}/.ssh/known_hosts"

                        SSH_OPTIONS=(
                            -i "$SSH_KEY"
                            -p "$SSH_PORT"
                            -o BatchMode=yes
                            -o StrictHostKeyChecking=yes
                            -o UserKnownHostsFile="$KNOWN_HOSTS"
                            -o ConnectTimeout=15
                        )

                        echo "Testing connection to ${DEPLOY_HOST}..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            'echo "SSH connection successful."'

                        echo "Preparing deployment directory..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "mkdir -p '${DEPLOY_DIR}/data'"

                        echo "Removing old application files while preserving data..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "find '${DEPLOY_DIR}' \
                                -mindepth 1 \
                                -maxdepth 1 \
                                ! -name data \
                                -exec rm -rf -- {} +"

                        echo "Copying repository files..."

                        tar \
                            --exclude='.git' \
                            --exclude='.github' \
                            --exclude='.idea' \
                            --exclude='.vscode' \
                            --exclude='.DS_Store' \
                            --exclude='cmake-build-*' \
                            --exclude='build' \
                            --exclude='data' \
                            --exclude='*.log' \
                            -czf - . |
                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "cd '${DEPLOY_DIR}' && tar -xzf -"

                        echo "Copying initial data files without overwriting existing data..."

                        if [[ -d data ]]; then
                            tar -C data -czf - . |
                            ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                                "cd '${DEPLOY_DIR}/data' &&
                                 tar --skip-old-files -xzf -"
                        fi

                        echo "Starting Docker Compose deployment..."

                        ssh "${SSH_OPTIONS[@]}" "$TARGET" \
                            "cd '${DEPLOY_DIR}' &&
                             echo 'Validating Compose configuration...' &&
                             docker compose config --quiet &&
                             echo 'Building services with Docker BuildKit...' &&
                             DOCKER_BUILDKIT=1 COMPOSE_DOCKER_CLI_BUILD=1 docker compose build &&
                             echo 'Starting services...' &&
                             DOCKER_BUILDKIT=1 COMPOSE_DOCKER_CLI_BUILD=1 docker compose up --detach --remove-orphans &&
                             echo 'Service status:' &&
                             docker compose ps"
                    '''
                }
            }
        }
    }

    post {
        success {
            echo 'Deployment completed successfully.'
        }

        failure {
            echo 'Deployment failed. Review the Jenkins console output.'
        }
    }
}
